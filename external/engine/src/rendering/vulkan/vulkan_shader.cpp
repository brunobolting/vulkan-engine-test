#include "vulkan_shader.h"
#include "vulkan_utilities.h"
#include "vulkan_initializers.h"
#include "vulkan_pipeline_builder.h"
#include "vulkan_types.h"

namespace ZERO
{
    VulkanShader::VulkanShader(VulkanRenderer *renderer) : _renderer(renderer) {}

    VulkanShader::~VulkanShader() {
        cleanPipelineObjects();
    }

    void VulkanShader::Bind() {
        vkCmdBindPipeline(_renderer->_mainCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);
    }

    void VulkanShader::Load(const std::string &&vertexShader, const std::string &&fragmentShader) {
        cleanPipelineObjects();
        _vertexShaderLocation = vertexShader;
        _fragmentShaderLocation = fragmentShader;
        VkShaderModule vertShader;
        if (!VulkanUtilities::LoadShaderModule(vertexShader, _renderer->_device, vertShader)) {
            printf("failed to load vertex shader module: %s\n", vertexShader.c_str());
        }

        VkShaderModule fragShader;
        if (!VulkanUtilities::LoadShaderModule(fragmentShader, _renderer->_device, fragShader)) {
            printf("failed to load fragment shader module: %s\n", fragmentShader.c_str());
        }

        auto pipelineLayoutCreateInfo = VulkanInitializers::PipelineLayoutCreateInfo();
        VK_CHECK(vkCreatePipelineLayout(_renderer->_device, &pipelineLayoutCreateInfo, nullptr, &_pipelineLayout));

        VulkanPipelineBuilder pipelineBuilder;
        pipelineBuilder._shaderStages.push_back(VulkanInitializers::PipelineShaderStageCreateInfo(
            VK_SHADER_STAGE_VERTEX_BIT,
            vertShader
        ));
        pipelineBuilder._shaderStages.push_back(VulkanInitializers::PipelineShaderStageCreateInfo(
            VK_SHADER_STAGE_FRAGMENT_BIT,
            fragShader
        ));
        auto vertexBindings = GetVertexInputDescription();
        pipelineBuilder._vertexInputInfo = VulkanInitializers::PipelineVertexInputStateCreateInfo();
        pipelineBuilder._vertexInputInfo.vertexBindingDescriptionCount = vertexBindings.Bindings.size();
        pipelineBuilder._vertexInputInfo.pVertexBindingDescriptions = vertexBindings.Bindings.data();
        pipelineBuilder._vertexInputInfo.vertexAttributeDescriptionCount = vertexBindings.Attributes.size();
        pipelineBuilder._vertexInputInfo.pVertexAttributeDescriptions = vertexBindings.Attributes.data();
        pipelineBuilder._inputAssembly = VulkanInitializers::PipelineInputAssemblyStateCreateInfo(
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
        );
        pipelineBuilder._viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = (float) _renderer->_windowExtent.width,
            .height = (float) _renderer->_windowExtent.height,
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        };
        pipelineBuilder._scissor = {
            .offset = {
                .x = 0,
                .y = 0
            },
            .extent = _renderer->_windowExtent
        };
        pipelineBuilder._rasterizer = VulkanInitializers::PipelineRasterizationStateCreateInfo(
            VK_POLYGON_MODE_FILL
        );
        pipelineBuilder._multisampling = VulkanInitializers::PipelineMultisampleStateCreateInfo();
        pipelineBuilder._colorBlendAttachment = VulkanInitializers::PipelineColorBlendAttachmentState();
        pipelineBuilder._pipelineLayout = _pipelineLayout;

        _pipeline = pipelineBuilder.BuildPipeline(_renderer->_device, _renderer->_renderPass);

        vkDestroyShaderModule(_renderer->_device, vertShader, nullptr);
        vkDestroyShaderModule(_renderer->_device, fragShader, nullptr);
    }

    void VulkanShader::Rebuild() {
        Load(std::move(_vertexShaderLocation), std::move(_fragmentShaderLocation));
    }

    void VulkanShader::cleanPipelineObjects() {
        if (_pipelineLayout != VK_NULL_HANDLE) {
            vkDestroyPipelineLayout(_renderer->_device, _pipelineLayout, nullptr);
            _pipelineLayout = VK_NULL_HANDLE;
        }
        if (_pipeline != VK_NULL_HANDLE) {
            vkDestroyPipeline(_renderer->_device, _pipeline, nullptr);
            _pipeline = VK_NULL_HANDLE;
        }
    }
}
