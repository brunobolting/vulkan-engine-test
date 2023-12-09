#include "vulkan_initializers.h"

namespace ZERO
{
    VkCommandPoolCreateInfo VulkanInitializers::CommandPoolCreateInfo(
        uint32_t queueFamilyIndex,
        VkCommandPoolCreateFlags flag
    ) {
        VkCommandPoolCreateInfo commandPoolCreateInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
        commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
        commandPoolCreateInfo.flags = flag;
        return commandPoolCreateInfo;
    }

    VkCommandBufferAllocateInfo VulkanInitializers::CommandBufferAllocateInfo(
        VkCommandPool pool,
        uint32_t count,
        VkCommandBufferLevel level
    ) {
        VkCommandBufferAllocateInfo commandBufferAllocateInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
        commandBufferAllocateInfo.commandPool = pool;
        commandBufferAllocateInfo.commandBufferCount = count;
        commandBufferAllocateInfo.level = level;
        return commandBufferAllocateInfo;
    }

    VkPipelineShaderStageCreateInfo VulkanInitializers::PipelineShaderStageCreateInfo(
        VkShaderStageFlagBits stage,
        VkShaderModule shaderModule
    ) {
        VkPipelineShaderStageCreateInfo shaderStageCreateInfo { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
        shaderStageCreateInfo.stage = stage;
        shaderStageCreateInfo.module = shaderModule;
        shaderStageCreateInfo.pName = "main";
        return shaderStageCreateInfo;
    }

    VkPipelineVertexInputStateCreateInfo VulkanInitializers::PipelineVertexInputStateCreateInfo() {
        VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo {
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO
        };
        vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
        vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
        return vertexInputCreateInfo;
    }

    VkPipelineInputAssemblyStateCreateInfo VulkanInitializers::PipelineInputAssemblyStateCreateInfo(
        VkPrimitiveTopology topology
    ) {
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo {
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO
        };
        inputAssemblyCreateInfo.topology = topology;
        inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;
        return inputAssemblyCreateInfo;
    }

    VkPipelineRasterizationStateCreateInfo VulkanInitializers::PipelineRasterizationStateCreateInfo(
        VkPolygonMode polygonMode
    ) {
        VkPipelineRasterizationStateCreateInfo rasterizationCreateInfo {
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO
        };
        rasterizationCreateInfo.depthClampEnable = VK_FALSE;
        rasterizationCreateInfo.rasterizerDiscardEnable = VK_FALSE;
        rasterizationCreateInfo.polygonMode = polygonMode;
        rasterizationCreateInfo.lineWidth = 1.0f;
        rasterizationCreateInfo.cullMode = VK_CULL_MODE_NONE;
        rasterizationCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizationCreateInfo.depthBiasEnable = VK_FALSE;
        rasterizationCreateInfo.depthBiasConstantFactor = 0.0f;
        rasterizationCreateInfo.depthBiasClamp = 0.0f;
        rasterizationCreateInfo.depthBiasSlopeFactor = 0.0f;
        return rasterizationCreateInfo;
    }

    VkPipelineMultisampleStateCreateInfo VulkanInitializers::PipelineMultisampleStateCreateInfo() {
        VkPipelineMultisampleStateCreateInfo multisampleCreateInfo {
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO
        };
        multisampleCreateInfo.sampleShadingEnable = VK_FALSE;
        multisampleCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampleCreateInfo.minSampleShading = 1.0f;
        multisampleCreateInfo.pSampleMask = nullptr;
        multisampleCreateInfo.alphaToCoverageEnable = VK_FALSE;
        multisampleCreateInfo.alphaToOneEnable = VK_FALSE;
        return multisampleCreateInfo;
    }

    VkPipelineColorBlendAttachmentState VulkanInitializers::PipelineColorBlendAttachmentState() {
        VkPipelineColorBlendAttachmentState colorBlendAttachmentState {};
        colorBlendAttachmentState.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT |
            VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachmentState.blendEnable = VK_FALSE;
        return colorBlendAttachmentState;
    }

    VkPipelineLayoutCreateInfo VulkanInitializers::PipelineLayoutCreateInfo() {
        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo {
            VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO
        };
        pipelineLayoutCreateInfo.flags = 0;
        pipelineLayoutCreateInfo.setLayoutCount = 0;
        pipelineLayoutCreateInfo.pSetLayouts = nullptr;
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
        pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
        return pipelineLayoutCreateInfo;
    }
}
