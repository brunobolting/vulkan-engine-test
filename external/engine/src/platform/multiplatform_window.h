#pragma once

#include <engine/platform/window.h>
#include <GLFW/glfw3.h>

class MultiplatformWindow: public Window
{
    public:
        MultiplatformWindow();
        void OpenWindow(WindowData data) override;
        bool Update() override;
    private:
        GLFWwindow* _window;
};
