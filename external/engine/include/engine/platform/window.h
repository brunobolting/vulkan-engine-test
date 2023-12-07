#pragma once

#include <string>
#include <unordered_map>
#include <any>
#include <tuple>

namespace ZERO
{
    enum class SurfaceArgs
    {
        INSTANCE,
        ALLOCATORS,
        OUT_SURFACE
    };

    struct WindowData
    {
        std::string title;
        uint32_t width;
        uint32_t height;
    };

    class Window
    {
        public:
            virtual void OpenWindow(WindowData data) = 0;
            virtual bool Update() = 0;
            virtual std::pair<int, int> GetWindowExtents() = 0;
            virtual void RequestDrawSurface(std::unordered_map<SurfaceArgs, std::any>) = 0;
    };
}
