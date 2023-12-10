#pragma once

#include <string>

namespace ZERO
{
    class Shader
    {
        public:
            virtual ~Shader() = default;
            virtual void Bind() = 0;
            virtual void Load(const std::string &&vertexShader, const std::string &&fragmentShader) = 0;
    };
}
