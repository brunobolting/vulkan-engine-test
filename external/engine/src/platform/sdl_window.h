#pragma once

#include "engine/platform/window.h"
#include <SDL3/SDL.h>

namespace ZERO
{
    class SDLWindow : public Window
    {
        public:
            SDLWindow() = default;
            void OpenWindow(WindowData data) override;
            bool Update() override;
            std::pair<int, int> GetWindowExtents() override;
            void RequestDrawSurface(std::unordered_map<SurfaceArgs, int*> args) override;

        private:
            SDL_Window *_window = nullptr;
    };
}
