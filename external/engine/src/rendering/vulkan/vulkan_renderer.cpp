#include <VkBootstrap.h>
#include <engine/service_locator.h>
#include "vulkan_renderer.h"
#include "vulkan_initializers.h"
#include "vulkan_types.h"
#include "vulkan_utilities.h"
#include <cmath>

namespace ZERO
{
    const uint64_t ONE_SECOND = 1e9;

    void VulkanRenderer::Init(RendererSettings settings) {
        _rendererSettings = settings;
        initCore();
        createSwapchain();
        createCommands();
        createDefaultRenderPass();
        createFramebuffers();
        createSyncStructures();
    }

    void VulkanRenderer::Shutdown() {
        vkDeviceWaitIdle(_device);
        vkDestroyFence(_device, _renderFence, nullptr);
        vkDestroySemaphore(_device, _presentSemaphore, nullptr);
        vkDestroySemaphore(_device, _renderSemaphore, nullptr);
        for (auto framebuffer : _framebuffers) {
            vkDestroyFramebuffer(_device, framebuffer, nullptr);
        }
        vkDestroyRenderPass(_device, _renderPass, nullptr);
        vkDestroyCommandPool(_device, _commandPool, nullptr);
        vkDestroySwapchainKHR(_device, _swapchain, nullptr);
        for (auto imageView : _swapchainImageViews) {
            vkDestroyImageView(_device, imageView, nullptr);
        }
        vkDestroyDevice(_device, nullptr);
        vkDestroySurfaceKHR(_instance, _surface, nullptr);
        vkb::destroy_debug_utils_messenger(_instance, _debugMessenger);
        vkDestroyInstance(_instance, nullptr);
    }

    void VulkanRenderer::RenderFrame() {
        VK_CHECK(vkWaitForFences(_device, 1, &_renderFence, true, ONE_SECOND));
        VK_CHECK(vkResetFences(_device, 1, &_renderFence));

        uint32_t swapchainImageIndex;
        VK_CHECK(vkAcquireNextImageKHR(
            _device,
            _swapchain,
            ONE_SECOND,
            _presentSemaphore,
            nullptr,
            &swapchainImageIndex
        ));
        VK_CHECK(vkResetCommandBuffer(_mainCommandBuffer, 0));

        VkCommandBuffer cmd = _mainCommandBuffer;
        VkCommandBufferBeginInfo beginInfo { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        VK_CHECK(vkBeginCommandBuffer(cmd, &beginInfo));

        float flashColor = std::abs(std::sin((float)_frameNumber / 120.0f));
        VkClearValue clearValue {
            .color = { 0.f, 0.f, flashColor, 1.0f }
        };

        VkRenderPassBeginInfo renderPassBeginInfo { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
        renderPassBeginInfo.renderPass = _renderPass;
        renderPassBeginInfo.renderArea = {
            .offset = {
                .x = 0,
                .y = 0
            },
            .extent = _windowExtent
        };
        renderPassBeginInfo.framebuffer = _framebuffers[swapchainImageIndex];
        // connect clear values
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearValue;

        vkCmdBeginRenderPass(cmd, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        // DRAW CALLS

        vkCmdEndRenderPass(cmd);
        VK_CHECK(vkEndCommandBuffer(cmd));

        VkSubmitInfo submit { VK_STRUCTURE_TYPE_SUBMIT_INFO };
        VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        submit.pWaitDstStageMask = &waitStage;
        submit.waitSemaphoreCount = 1;
        submit.pWaitSemaphores = &_presentSemaphore;
        submit.signalSemaphoreCount = 1;
        submit.pSignalSemaphores = &_renderSemaphore;
        submit.commandBufferCount = 1;
        submit.pCommandBuffers = &_mainCommandBuffer;

        VK_CHECK(vkQueueSubmit(_graphicsQueue, 1, &submit, _renderFence));

        VkPresentInfoKHR presentInfoKhr { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
        presentInfoKhr.swapchainCount = 1;
        presentInfoKhr.pSwapchains = &_swapchain;
        presentInfoKhr.waitSemaphoreCount = 1;
        presentInfoKhr.pWaitSemaphores = &_renderSemaphore;
        presentInfoKhr.pImageIndices = &swapchainImageIndex;
        VK_CHECK(vkQueuePresentKHR(_graphicsQueue, &presentInfoKhr));
        _frameNumber++;
    }

    void VulkanRenderer::initCore() {
        // Initialize the instance
        vkb::InstanceBuilder builder;
        auto builderInstance = builder.set_app_name(_rendererSettings.ApplicationName.c_str())
            .request_validation_layers(true)
            .require_api_version(1, 3, 0)
            .use_default_debug_messenger()
            .build();
        vkb::Instance vulkan = builderInstance.value();
        _instance = vulkan.instance;
        _debugMessenger = vulkan.debug_messenger;

        // request vulkan surface
        ServiceLocator::GetWindow()->RequestDrawSurface({
            {SurfaceArgs::INSTANCE, _instance},
            {SurfaceArgs::OUT_SURFACE, &_surface}
        });

        // Select a physical device
        vkb::PhysicalDeviceSelector selector { vulkan };
        vkb::PhysicalDevice physicalDevice {
            selector
                .set_minimum_version(1, 1)
                .set_surface(_surface)
                .select()
                .value()
        };
        vkb::DeviceBuilder deviceBuilder { physicalDevice };
        vkb::Device device { deviceBuilder.build().value() };
        _device = device.device;
        _physicalDevice = physicalDevice.physical_device;
        _graphicsQueue = device.get_queue(vkb::QueueType::graphics).value();
        _graphicsQueueFamily = device.get_queue_index(vkb::QueueType::graphics).value();
    }

    void VulkanRenderer::createSwapchain() {
        auto [width, height] = ServiceLocator::GetWindow()->GetWindowExtents();
        _windowExtent.width = width;
        _windowExtent.height = height;
        vkb::SwapchainBuilder swapchainBuilder { _physicalDevice, _device, _surface };
        vkb::Swapchain vkbSwapchain = swapchainBuilder
            .use_default_format_selection()
            .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR) // Hard VSync
            .set_desired_extent(width, height)
            .build()
            .value();
        _swapchain = vkbSwapchain.swapchain;
        _swapchainImages = vkbSwapchain.get_images().value();
        _swapchainImageViews = vkbSwapchain.get_image_views().value();
        _swapchainImageFormat = vkbSwapchain.image_format;
    }

    void VulkanRenderer::createCommands() {
        VkCommandPoolCreateInfo commandPoolCreateInfo = VulkanInitializers::CommandPoolCreateInfo(
            _graphicsQueueFamily,
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
        );
        VK_CHECK(vkCreateCommandPool(_device, &commandPoolCreateInfo, nullptr, &_commandPool));

        VkCommandBufferAllocateInfo commandBufferAllocateInfo = VulkanInitializers::CommandBufferAllocateInfo(
            _commandPool
        );
        VK_CHECK(vkAllocateCommandBuffers(_device, &commandBufferAllocateInfo, &_mainCommandBuffer));
    }

    void VulkanRenderer::createDefaultRenderPass() {
        VkAttachmentDescription colorAttachment {
            .format = _swapchainImageFormat,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        };
        VkAttachmentReference colorAttachmentRef {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };
        VkSubpassDescription subpass {
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount = 1,
            .pColorAttachments = &colorAttachmentRef
        };
        VkRenderPassCreateInfo renderPassCreateInfo { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
        renderPassCreateInfo.attachmentCount = 1;
        renderPassCreateInfo.pAttachments = &colorAttachment;
        renderPassCreateInfo.subpassCount = 1;
        renderPassCreateInfo.pSubpasses = &subpass;

        VK_CHECK(vkCreateRenderPass(_device, &renderPassCreateInfo, nullptr, &_renderPass));
    }

    void VulkanRenderer::createFramebuffers() {
        VkFramebufferCreateInfo framebufferCreateInfo { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
        framebufferCreateInfo.renderPass = _renderPass;
        framebufferCreateInfo.attachmentCount = 1;
        framebufferCreateInfo.width = _windowExtent.width;
        framebufferCreateInfo.height = _windowExtent.height;
        framebufferCreateInfo.layers = 1;

        const uint32_t swapchainImageCount = _swapchainImages.size();
        _framebuffers.resize(swapchainImageCount);

        for (int i = 0; i < swapchainImageCount; i++) {
            framebufferCreateInfo.pAttachments = &_swapchainImageViews[i];
            VK_CHECK(vkCreateFramebuffer(_device, &framebufferCreateInfo, nullptr, &_framebuffers[i]));
        }
    }

    void VulkanRenderer::createSyncStructures() {
        VkFenceCreateInfo fenceCreateInfo { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        VK_CHECK(vkCreateFence(_device, &fenceCreateInfo, nullptr, &_renderFence));

        VkSemaphoreCreateInfo semaphoreCreateInfo { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
        VK_CHECK(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_presentSemaphore));
        VK_CHECK(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_renderSemaphore));
    }
}
