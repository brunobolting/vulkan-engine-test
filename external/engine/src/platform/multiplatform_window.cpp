#include <stdexcept>
#include <iostream>

#include "multiplatform_window.h"
#include <engine/service_locator.h>

namespace ZERO
{
    MultiplatformWindow::MultiplatformWindow() {
        _window = nullptr;
    }

    void MultiplatformWindow::OpenWindow(WindowData data) {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        _window = glfwCreateWindow(data.width, data.height, data.title.c_str(), nullptr, nullptr);

        glfwSetWindowUserPointer(_window, &_input);

        // TODO: refactor this callbacks
        // register some callbacks

        glfwSetKeyCallback(_window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            auto *input = static_cast<MultiplatformInput*>(glfwGetWindowUserPointer(window));
            if (!input) {
                return;
            }
            float value = 0.f;
            switch (action) {
                case GLFW_PRESS:
                case GLFW_REPEAT:
                    value = 1.f;
                    break;
                default:
                    value = 0.f;
                    break;
            }
            input->UpdateKeyboardState(key, value);
        });

        glfwSetMouseButtonCallback(_window, [](GLFWwindow *window, int button, int action, int mods) {
            auto *input = static_cast<MultiplatformInput*>(glfwGetWindowUserPointer(window));
            if (!input) {
                return;
            }
            float value = 0.f;
            switch (action) {
                case GLFW_PRESS:
                case GLFW_REPEAT:
                    value = 1.f;
                    break;
                default:
                    value = 0.f;
                    break;
            }
            input->UpdateMouseState(button, value);
        });

        glfwSetJoystickCallback([](int joystickId, int event) {
            auto *inputManager = ServiceLocator::GetInputManager();
            if (!inputManager) {
                return;
            }
            auto *window = dynamic_cast<MultiplatformWindow*>(ServiceLocator::GetWindow());
            if (!window) {
                return;
            }
            if (event == GLFW_CONNECTED) {
                inputManager->RegisterDevice(InputDevice {
                    .Type = InputDeviceType::GAMEPAD,
                    .Index = joystickId,
                    .StateFunc = std::bind(&MultiplatformWindow::getGamepadState, window, std::placeholders::_1)
                });
                printf("Joystick %d is now connected\n", joystickId);
            } else if (event == GLFW_DISCONNECTED) {
                inputManager->RemoveDevice(InputDeviceType::GAMEPAD, joystickId);
                printf("Joystick %d is now disconnected\n", joystickId);
            }
        });

        glfwSetFramebufferSizeCallback(_window, [](GLFWwindow *window, int width, int height) {
            auto *glfwWindow = static_cast<MultiplatformWindow*>(ServiceLocator::GetWindow());
            if (glfwWindow->_resizeCallback) {
                glfwWindow->_resizeCallback();
            }
        });

        auto *inputManager = ServiceLocator::GetInputManager();
        inputManager->RegisterDevice(InputDevice {
            .Type = InputDeviceType::KEYBOARD,
            .Index = 0,
            .StateFunc = std::bind(&MultiplatformInput::GetKeyboardState, &_input, std::placeholders::_1),
        });
        inputManager->RegisterDevice(InputDevice {
            .Type = InputDeviceType::MOUSE,
            .Index = 0,
            .StateFunc = std::bind(&MultiplatformInput::GetMouseState, &_input, std::placeholders::_1),
        });
        for (int i = 0; i < GLFW_JOYSTICK_LAST; i++) {
            if (glfwJoystickPresent(i)) {
                inputManager->RegisterDevice(InputDevice {
                    .Type = InputDeviceType::GAMEPAD,
                    .Index = i,
                    .StateFunc = std::bind(&MultiplatformWindow::getGamepadState, this, std::placeholders::_1)
                });
            }
        }
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

    void MultiplatformWindow::RequestDrawSurface(std::unordered_map<SurfaceArgs, int*> args) {
        auto vkInstance = reinterpret_cast<VkInstance>(args[SurfaceArgs::INSTANCE]);
        auto outSurface = reinterpret_cast<VkSurfaceKHR *>(args[SurfaceArgs::OUT_SURFACE]);
        VkAllocationCallbacks* allocationCallbacks = nullptr;
        if (args[SurfaceArgs::ALLOCATORS]) {
            allocationCallbacks = reinterpret_cast<VkAllocationCallbacks *>(args[SurfaceArgs::ALLOCATORS]);
        }
        if (vkInstance == VK_NULL_HANDLE) {
            throw std::runtime_error("vulkan instance not provided");
        }
        if (glfwCreateWindowSurface(vkInstance, _window, allocationCallbacks, outSurface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface");
        }
    }

    std::unordered_map<InputKey, InputDeviceState> MultiplatformWindow::getGamepadState(int joystickId) {
        GLFWgamepadstate state;
        if (!glfwGetGamepadState(joystickId, &state)) {
            return std::unordered_map<InputKey, InputDeviceState>{};
        }
        return _input.GetGamepadState(state);
    }
}
