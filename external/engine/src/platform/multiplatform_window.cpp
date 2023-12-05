#include "multiplatform_window.h"

MultiplatformWindow::MultiplatformWindow() {
    _window = nullptr;
}

void MultiplatformWindow::OpenWindow(WindowData data) {
    glfwInit();
    _window = glfwCreateWindow(data.width, data.height, data.title.c_str(), nullptr, nullptr);
}

bool MultiplatformWindow::Update() {
    glfwPollEvents();
    return glfwWindowShouldClose(_window);
}
