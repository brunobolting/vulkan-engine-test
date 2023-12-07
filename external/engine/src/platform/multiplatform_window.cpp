#include "multiplatform_window.h"
#include <stdexcept>
#include <iostream>

namespace ZERO
{
    MultiplatformWindow::MultiplatformWindow() {
        _window = nullptr;
    }

    void MultiplatformWindow::OpenWindow(WindowData data) {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        _window = glfwCreateWindow(data.width, data.height, data.title.c_str(), nullptr, nullptr);
    }

    bool MultiplatformWindow::Update() {
        glfwPollEvents();
        return glfwWindowShouldClose(_window);
    }

    std::pair<int, int> MultiplatformWindow::GetWindowExtents() {
        int width;
        int height;
        glfwGetFramebufferSize(_window, &width, &height);
        return {width, height};
    }

    void MultiplatformWindow::RequestDrawSurface(std::unordered_map<SurfaceArgs, std::any> args) {
        try {
            auto vkInstance = std::any_cast<VkInstance>(args[SurfaceArgs::INSTANCE]);
            auto outSurface = std::any_cast<VkSurfaceKHR *>(args[SurfaceArgs::OUT_SURFACE]);
            VkAllocationCallbacks* allocationCallbacks = nullptr;
            if (args[SurfaceArgs::ALLOCATORS].has_value()) {
                allocationCallbacks = std::any_cast<VkAllocationCallbacks *>(args[SurfaceArgs::ALLOCATORS]);
            }
            if (vkInstance == VK_NULL_HANDLE) {
                throw std::runtime_error("vulkan instance not provided");
            }
            if (glfwCreateWindowSurface(vkInstance, _window, allocationCallbacks, outSurface) != VK_SUCCESS) {
                throw std::runtime_error("failed to create window surface");
            }
        } catch (const std::bad_any_cast &e) {
            std::cout << "Failed to cast window surface arguments: " << e.what() << std::endl;
        }
    }
}
