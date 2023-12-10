#pragma once

#include <engine/rendering/renderer.h>
#include <vulkan/vulkan.h>
#include <vector>

namespace ZERO
{
    class VulkanRenderer: public Renderer
    {
        friend class VulkanShader;
        /*
         * FUNCTIONS
        */
        public:
            void Init(RendererSettings settings) override;
            void Shutdown() override;
            void RenderFrame() override;
            std::shared_ptr<Shader> CreateShader() override;

        private:
            void initCore();
            void createSwapchain();
            void createCommands();
            void createDefaultRenderPass();
            void createFramebuffers();
            void createSyncStructures();
            void createPipelines();

        /*
         * MEMBERS
        */
        private:
            // TODO: temporary frame number
            uint64_t _frameNumber { 0 };
            RendererSettings _rendererSettings {};
            /*
             * CORE VULKAN
            */
            VkInstance _instance;
            VkDebugUtilsMessengerEXT _debugMessenger;
            VkPhysicalDevice _physicalDevice; //physical device
            VkDevice _device; // logical device
            VkSurfaceKHR _surface;
            /*
             * SWAPCHAIN
            */
            VkSwapchainKHR _swapchain;
            VkFormat _swapchainImageFormat;
            std::vector<VkImage> _swapchainImages;
            std::vector<VkImageView> _swapchainImageViews;
            VkExtent2D _windowExtent;
            /*
             * COMMAND POOLS AND QUEUES
            */
            VkQueue _graphicsQueue;
            uint32_t _graphicsQueueFamily;
            VkCommandPool _commandPool;
            VkCommandBuffer _mainCommandBuffer;
            /*
             * RENDER PASS
            */
            VkRenderPass _renderPass;
            std::vector<VkFramebuffer> _framebuffers { 3 };
            /*
             * SYNCHRONIZATION OBJECTS
            */
            VkSemaphore _presentSemaphore;
            VkSemaphore _renderSemaphore;
            VkFence _renderFence;
            /*
             * TEMPORARY RUNTIME GAME OBJECTS
            */
            std::shared_ptr<Shader> _triangleShader { nullptr };
            std::shared_ptr<Shader> _triangleShader2 { nullptr };
    };
}
