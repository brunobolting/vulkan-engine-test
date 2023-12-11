#pragma once

#include <engine/rendering/shader.h>
#include <engine/rendering/buffer.h>
#include <string>
#include <memory>

namespace ZERO
{
    struct RendererSettings
    {
        std::string ApplicationName;
    };

    class Renderer
    {
        public:
            virtual ~Renderer() = default;
            virtual void Init(RendererSettings) = 0;
            virtual void Shutdown() = 0;
            virtual void RenderFrame() = 0;
            virtual std::shared_ptr<Shader> CreateShader() = 0;
            virtual std::shared_ptr<VertexBuffer> CreateVertexBuffer() = 0;
            virtual std::shared_ptr<IndexBuffer> CreateIndexBuffer() = 0;
    };
}
