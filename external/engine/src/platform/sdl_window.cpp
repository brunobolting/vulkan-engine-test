#include <SDL3/SDL_vulkan.h>
#include <iostream>

#include "sdl_window.h"

namespace ZERO
{
    void SDLWindow::OpenWindow(WindowData data) {
        SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_EVENTS|SDL_INIT_JOYSTICK|SDL_INIT_HAPTIC|SDL_INIT_GAMEPAD|SDL_INIT_SENSOR);
        auto width = data.width;
        auto height = data.height;
        _window = SDL_CreateWindow(
            data.title.c_str(),
            width,
            height,
            SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE
        );
    }

    bool SDLWindow::Update() {
        SDL_Event event;
        SDL_PollEvent(&event);

        if (event.type == SDL_EVENT_QUIT) {
            return true;
        }

        switch (event.window.type) {
            case SDL_EVENT_WINDOW_RESIZED:
                if (_resizeCallback) {
                    _resizeCallback();
                }
                break;
            default:
                break;
        }

        return false;
    }

    std::pair<int, int> SDLWindow::GetWindowExtents() {
        int width;
        int height;
        SDL_GetWindowSizeInPixels(_window, &width, &height);
        return {width, height};
    }

    void SDLWindow::RequestDrawSurface(std::unordered_map<SurfaceArgs, int*> args) {
        auto vkInstance = reinterpret_cast<VkInstance>(args[SurfaceArgs::INSTANCE]);
        auto *outSurface = reinterpret_cast<VkSurfaceKHR *>(args[SurfaceArgs::OUT_SURFACE]);
        VkAllocationCallbacks *allocationCallbacks = nullptr;
        if (args[SurfaceArgs::ALLOCATORS]) {
            allocationCallbacks = reinterpret_cast<VkAllocationCallbacks *>(args[SurfaceArgs::ALLOCATORS]);
        }
        if (vkInstance == nullptr) {
            throw std::runtime_error("vulkan instance not provided");
        }
        if (SDL_Vulkan_CreateSurface(_window, vkInstance, allocationCallbacks, outSurface) != true) {
            throw std::runtime_error("failed to create window surface");
        }
    }
}
