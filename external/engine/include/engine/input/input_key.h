#pragma once

#include <string>

namespace ZERO
{
    enum class InputKey
    {
        UNKNOWN,
        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_S,
        KEY_W,
        GAMEPAD_A,
        GAMEPAD_B,
        GAMEPAD_X,
        GAMEPAD_Y,
        GAMEPAD_LEFT_THUMB,
        GAMEPAD_RIGHT_THUMB,
        GAMEPAD_LEFT_BUMPER,
        GAMEPAD_RIGHT_BUMPER,
        GAMEPAD_DPAD_UP,
        GAMEPAD_DPAD_RIGHT,
        GAMEPAD_DPAD_DOWN,
        GAMEPAD_DPAD_LEFT,
        GAMEPAD_START,
        GAMEPAD_BACK,
        GAMEPAD_GUIDE,
        GAMEPAD_LEFT_THUMB_X,
        GAMEPAD_LEFT_THUMB_Y,
        GAMEPAD_RIGHT_THUMB_X,
        GAMEPAD_RIGHT_THUMB_Y,
        GAMEPAD_LEFT_TRIGGER,
        GAMEPAD_RIGHT_TRIGGER,
        MOUSE_POS_X,
        MOUSE_POS_Y,
        MOUSE_MOVE_X,
        MOUSE_MOVE_Y,
        MOUSE_RIGHT,
        MOUSE_LEFT,
        MOUSE_MIDDLE,
    };

    enum class InputSource
    {
        KEYBOARD,
        MOUSE,
        GAMEPAD,
        UNKNOWN,
    };

    struct InputAction {
        std::string actionName { "" };
        float scale { 1.0f };
    };
}
