#pragma once

#include <functional>
#include <unordered_map>

#include "input_key.h"

namespace ZERO
{
    enum class InputDeviceType
    {
        KEYBOARD,
        MOUSE,
        GAMEPAD,
    };

    struct InputDeviceState
    {
        float value { -99.f };
    };

    using InputDeviceStateCallbackFunc = std::function<std::unordered_map<InputKey, InputDeviceState>(int)>;

    struct InputDevice
    {
        InputDeviceType Type;
        int Index;
        std::unordered_map<InputKey, InputDeviceState> CurrentState;
        InputDeviceStateCallbackFunc StateFunc;
    };
}
