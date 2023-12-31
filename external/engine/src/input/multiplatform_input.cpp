#include "multiplatform_input.h"

#include <GLFW/glfw3.h>

namespace ZERO
{
    void MultiplatformInput::UpdateKeyboardState(int key, float value) {
        InputKey iKey = multiplatformKeyToInputKey(key);
        _keyboardState[iKey].value = value;
    }

    void MultiplatformInput::UpdateMouseState(int button, float value) {
        InputKey iKey = multiplatformMouseButtonToInputKey(button);
        _mouseState[iKey].value = value;
    }

    std::unordered_map<InputKey, InputDeviceState> MultiplatformInput::GetGamepadState(const GLFWgamepadstate &state) {
        std::unordered_map<InputKey, InputDeviceState> gamepadState {};
        // get the buttons
        for (int i = 0; i < GLFW_GAMEPAD_BUTTON_LAST; i++) {
            int buttonState = state.buttons[i];
            float value = 0.f;
            if (buttonState == GLFW_PRESS) {
                value = 1.f;
            }
            switch (i) {
                case GLFW_GAMEPAD_BUTTON_A:
                    gamepadState[InputKey::GAMEPAD_A].value = value;
                    break;
                case GLFW_GAMEPAD_BUTTON_B:
                    gamepadState[InputKey::GAMEPAD_B].value = value;
                    break;
                case GLFW_GAMEPAD_BUTTON_X:
                    gamepadState[InputKey::GAMEPAD_X].value = value;
                    break;
                case GLFW_GAMEPAD_BUTTON_Y:
                    gamepadState[InputKey::GAMEPAD_Y].value = value;
                    break;
                case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER:
                    gamepadState[InputKey::GAMEPAD_LEFT_BUMPER].value = value;
                    break;
                case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER:
                    gamepadState[InputKey::GAMEPAD_RIGHT_BUMPER].value = value;
                    break;
                case GLFW_GAMEPAD_BUTTON_BACK:
                    gamepadState[InputKey::GAMEPAD_BACK].value = value;
                    break;
                case GLFW_GAMEPAD_BUTTON_START:
                    gamepadState[InputKey::GAMEPAD_START].value = value;
                    break;
                case GLFW_GAMEPAD_BUTTON_GUIDE:
                    gamepadState[InputKey::GAMEPAD_GUIDE].value = value;
                    break;
                case GLFW_GAMEPAD_BUTTON_LEFT_THUMB:
                    gamepadState[InputKey::GAMEPAD_LEFT_THUMB].value = value;
                    break;
                case GLFW_GAMEPAD_BUTTON_RIGHT_THUMB:
                    gamepadState[InputKey::GAMEPAD_RIGHT_THUMB].value = value;
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_UP:
                    gamepadState[InputKey::GAMEPAD_DPAD_UP].value = value;
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT:
                    gamepadState[InputKey::GAMEPAD_DPAD_RIGHT].value = value;
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_DOWN:
                    gamepadState[InputKey::GAMEPAD_DPAD_DOWN].value = value;
                    break;
                case GLFW_GAMEPAD_BUTTON_DPAD_LEFT:
                    gamepadState[InputKey::GAMEPAD_DPAD_LEFT].value = value;
                    break;
                default:
                    break;
            }
        }

        // get the axes
        for (int i = 0; i < GLFW_GAMEPAD_AXIS_LAST; i++) {
            float value = state.axes[i];
            switch (i) {
                case GLFW_GAMEPAD_AXIS_LEFT_X:
                    gamepadState[InputKey::GAMEPAD_LEFT_THUMB_X].value = value;
                    break;
                case GLFW_GAMEPAD_AXIS_LEFT_Y:
                    gamepadState[InputKey::GAMEPAD_LEFT_THUMB_Y].value = -value;
                    break;
                case GLFW_GAMEPAD_AXIS_RIGHT_X:
                    gamepadState[InputKey::GAMEPAD_RIGHT_THUMB_X].value = value;
                    break;
                case GLFW_GAMEPAD_AXIS_RIGHT_Y:
                    gamepadState[InputKey::GAMEPAD_RIGHT_THUMB_Y].value = -value;
                    break;
                case GLFW_GAMEPAD_AXIS_LEFT_TRIGGER:
                    gamepadState[InputKey::GAMEPAD_LEFT_TRIGGER].value = value;
                    break;
                case GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER:
                    gamepadState[InputKey::GAMEPAD_RIGHT_TRIGGER].value = value;
                    break;
                default:
                    break;
            }
        }

        return gamepadState;
    }


    InputKey MultiplatformInput::multiplatformKeyToInputKey(int key) {
        switch (key) {
            case GLFW_KEY_A:
                return InputKey::KEY_A;
            case GLFW_KEY_B:
                return InputKey::KEY_B;
            case GLFW_KEY_C:
                return InputKey::KEY_C;
            case GLFW_KEY_D:
                return InputKey::KEY_D;
            case GLFW_KEY_E:
                return InputKey::KEY_E;
            case GLFW_KEY_S:
                return InputKey::KEY_S;
            case GLFW_KEY_W:
                return InputKey::KEY_W;
            default:
                return InputKey::UNKNOWN;
        }
    }

    InputKey MultiplatformInput::multiplatformMouseButtonToInputKey(int button) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                return InputKey::MOUSE_LEFT;
            case GLFW_MOUSE_BUTTON_RIGHT:
                return InputKey::MOUSE_RIGHT;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                return InputKey::MOUSE_MIDDLE;
            default:
                return InputKey::UNKNOWN;
        }
    }
}
