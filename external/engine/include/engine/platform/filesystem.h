#pragma once

#include <filesystem>

namespace ZERO
{
    using Path = std::filesystem::path;

    class Filesystem
    {
        public:
            static inline Path GetRootPath() {
                return std::filesystem::current_path();
            }

            static inline Path GetShaderPath() {
                return GetRootPath() /= "shaders";
            }
    };
}
