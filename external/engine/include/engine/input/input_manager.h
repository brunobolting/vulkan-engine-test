#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <functional>

#include "input_key.h"
#include "input_devices.h"

namespace ZERO
{
    class InputManager
    {
        public:
            using ActionCallbackFunc = std::function<bool(InputSource, int, float)>;
            struct ActionCallback
            {
                std::string Ref;
                ActionCallbackFunc Func;
            };

        private:
            struct ActionEvent
            {
                std::string ActionName;
                InputSource Source;
                int SourceIndex;
                float Value;
            };

        public:
            InputManager();
            ~InputManager();
            void RegisterDevice(const InputDevice &device);
            void RemoveDevice(InputDeviceType type, int inputIndex);
            void RegisterActionCallback(const std::string &actionName, const ActionCallback &callback);
            void RemoveActionCallback(const std::string &actionName, const std::string &callbackRef);
            void MapInputToAction(InputKey key, const InputAction &action);
            void UnmapInputFromAction(InputKey key, const std::string &action);
            InputSource GetInputSourceFromKey(InputKey key);

        private:
            friend class Game;
            // ProcessInput will get new device state and compare with old state;
            // then generate action events
            void processInput();
            std::vector<ActionEvent> generateActionEvent(int deviceIndex, InputKey key, float value);
            void propagateActionEvent(const ActionEvent &event);

        private:
            bool _active { false };
            std::unordered_map<InputKey, std::vector<InputAction>> _inputActionMapping {};
            std::unordered_map<std::string, std::vector<ActionCallback>> _actionCallbacks {};
            std::vector<InputDevice> _devices {};
    };
}
