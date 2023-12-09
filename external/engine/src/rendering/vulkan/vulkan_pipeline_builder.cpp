#include "vulkan_pipeline_builder.h"
#include <stdio.h>

namespace ZERO
{
    VkPipeline VulkanPipelineBuilder::BuildPipeline(VkDevice device, VkRenderPass renderPass) {
        VkPipelineViewportStateCreateInfo viewportState { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
        viewportState.viewportCount = 1;
        viewportState.pViewports = &_viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &_scissor;

        VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo {
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO
        };
        colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
        colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
        colorBlendStateCreateInfo.attachmentCount = 1;
        colorBlendStateCreateInfo.pAttachments = &_colorBlendAttachment;

        VkGraphicsPipelineCreateInfo pipelineCreateInfo { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
        pipelineCreateInfo.stageCount = _shaderStages.size();
        pipelineCreateInfo.pStages = _shaderStages.data();
        pipelineCreateInfo.pVertexInputState = &_vertexInputInfo;
        pipelineCreateInfo.pInputAssemblyState = &_inputAssembly;
        pipelineCreateInfo.pViewportState = &viewportState;
        pipelineCreateInfo.pRasterizationState = &_rasterizer;
        pipelineCreateInfo.pMultisampleState = &_multisampling;
        pipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
        pipelineCreateInfo.layout = _pipelineLayout;
        pipelineCreateInfo.renderPass = renderPass;
        pipelineCreateInfo.subpass = 0;
        pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

        VkPipeline pipeline;
        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &pipeline) != VK_SUCCESS) {
            printf("Failed to create pipeline\n");
            return VK_NULL_HANDLE;
        }
        return pipeline;
    }
}

