#pragma once

#include <engine/rendering/shader.h>
#include "vulkan_renderer.h"

namespace ZERO
{
    class VulkanShader : public Shader
    {
        public:
            VulkanShader(VulkanRenderer *renderer);
            ~VulkanShader();
            void Bind() override;
            void Load(const std::string &&vertexShader, const std::string &&fragmentShader) override;

        private:
            VulkanRenderer *_renderer { nullptr };

            // Vulkan handles
            VkPipelineLayout _pipelineLayout { VK_NULL_HANDLE };
            VkPipeline _pipeline { VK_NULL_HANDLE };
    };
}
