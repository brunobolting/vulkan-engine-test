#include "engine/input/input_manager.h"

#include <utility>

namespace ZERO
{
    InputSource InputManager::GetInputSourceFromKey(InputKey key) {
        switch (key) {
            case InputKey::KEY_A:
            case InputKey::KEY_B:
            case InputKey::KEY_C:
            case InputKey::KEY_D:
            case InputKey::KEY_E:
            case InputKey::KEY_S:
            case InputKey::KEY_W:
                return InputSource::KEYBOARD;
            case InputKey::GAMEPAD_A:
            case InputKey::GAMEPAD_B:
            case InputKey::GAMEPAD_X:
            case InputKey::GAMEPAD_Y:
            case InputKey::GAMEPAD_LEFT_THUMB:
            case InputKey::GAMEPAD_RIGHT_THUMB:
            case InputKey::GAMEPAD_LEFT_BUMPER:
            case InputKey::GAMEPAD_RIGHT_BUMPER:
            case InputKey::GAMEPAD_DPAD_UP:
            case InputKey::GAMEPAD_DPAD_RIGHT:
            case InputKey::GAMEPAD_DPAD_DOWN:
            case InputKey::GAMEPAD_DPAD_LEFT:
            case InputKey::GAMEPAD_START:
            case InputKey::GAMEPAD_BACK:
            case InputKey::GAMEPAD_GUIDE:
            case InputKey::GAMEPAD_LEFT_THUMB_X:
            case InputKey::GAMEPAD_LEFT_THUMB_Y:
            case InputKey::GAMEPAD_RIGHT_THUMB_X:
            case InputKey::GAMEPAD_RIGHT_THUMB_Y:
            case InputKey::GAMEPAD_LEFT_TRIGGER:
            case InputKey::GAMEPAD_RIGHT_TRIGGER:
                return InputSource::GAMEPAD;
            case InputKey::MOUSE_LEFT:
            case InputKey::MOUSE_RIGHT:
            case InputKey::MOUSE_MIDDLE:
            case InputKey::MOUSE_MOVE_X:
            case InputKey::MOUSE_MOVE_Y:
                return InputSource::MOUSE;
            default:
                return InputSource::UNKNOWN;
        }
    }

    InputManager::InputManager() {
        _active = true;
        printf("InputManager initialized\n");
    }

    InputManager::~InputManager() {
        _active = false;
    }

    void InputManager::RegisterActionCallback(const std::string &actionName, const ActionCallback &callback) {
        _actionCallbacks[actionName].emplace_back(callback);
    }

    void InputManager::RemoveActionCallback(const std::string &actionName, const std::string &callbackRef) {
        erase_if(_actionCallbacks[actionName], [callbackRef](const ActionCallback &callback) {
            return callback.Ref == callbackRef;
        });
    }

    void InputManager::MapInputToAction(InputKey key, const InputAction &action) {
        // TODO: check for duplicates
        _inputActionMapping[key].emplace_back(action);
    }

    void InputManager::UnmapInputFromAction(InputKey key, const std::string &action) {
        erase_if(_inputActionMapping[key], [action](const InputAction &inputAction) {
            return inputAction.actionName == action;
        });
    }

    void InputManager::processInput() {
        std::vector<ActionEvent> events {};
        for (auto &device : _devices) {
            // TODO: fix cases wherer conflicting mappings
            auto newState = device.StateFunc(device.Index);
            // compare to old state for device
            for (auto &keyState : newState) {
                if (device.CurrentState[keyState.first].value != keyState.second.value) {
                    auto generatedEvents = generateActionEvent(device.Index, keyState.first, keyState.second.value);
                    events.insert(events.end(), generatedEvents.begin(), generatedEvents.end());
                    // save new state value
                    device.CurrentState[keyState.first].value = keyState.second.value;
                }
            }
        }

        // propagate action events
        for (auto &event : events) {
            propagateActionEvent(event);
        }
    }

    std::vector<InputManager::ActionEvent> InputManager::generateActionEvent(int deviceIndex, InputKey key, float value) {
        std::vector<ActionEvent> actionEvents {};
        auto &actions = _inputActionMapping[key];
        InputSource source = GetInputSourceFromKey(key);
        for (auto &action : actions) {
            actionEvents.emplace_back(ActionEvent {
                .ActionName = action.actionName,
                .Source = source,
                .SourceIndex = deviceIndex,
                .Value = value * action.scale,
            });
        }
        return actionEvents;
    }

    void InputManager::propagateActionEvent(const ActionEvent &event) {
        for (int i = static_cast<int>(_actionCallbacks[event.ActionName].size()) - 1; i >= 0; i--) {
            bool ok = _actionCallbacks[event.ActionName][static_cast<size_t>(i)].Func(event.Source, event.SourceIndex, event.Value);
            if (ok) {
                break;
            }
        }
    }

    void InputManager::RegisterDevice(const InputDevice &device) {
        printf("Device registered of type %d\n", static_cast<int>(device.Type));
        _devices.emplace_back(device);
    }

    void InputManager::RemoveDevice(InputDeviceType type, int inputIndex) {
        erase_if(_devices, [type, inputIndex](const InputDevice &device) {
            return device.Type == type && device.Index == inputIndex;
        });
        printf("Device removed\n");
    }
}

