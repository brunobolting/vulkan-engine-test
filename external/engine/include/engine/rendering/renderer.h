#pragma once

#include <string>

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
    };
}
