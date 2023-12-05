#pragma once

#include <string>

struct WindowData {
    std::string title;
    uint32_t width;
    uint32_t height;
};

class Window
{
    public:
        virtual void OpenWindow(WindowData data) = 0;
        virtual bool Update() = 0;
};
