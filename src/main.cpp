#include <engine/platform/entry_point.h>
#include <engine/platform/game.h>
#include <engine/service_locator.h>

class CppGame: public ZERO::Game
{
    public:
        explicit CppGame(std::string title): Game(std::move(title)) {
            std::cout << "Hello, Cpp Game" << std::endl;

            _inputManager = ZERO::ServiceLocator::GetInputManager();
            if (_inputManager) {
                _inputManager->MapInputToAction(ZERO::InputKey::KEY_A, ZERO::InputAction {
                    .actionName = "strafe",
                    .scale = -1.0f,
                });
                _inputManager->MapInputToAction(ZERO::InputKey::KEY_D, ZERO::InputAction {
                    .actionName = "strafe",
                    .scale = 1.0f,
                });
                _inputManager->MapInputToAction(ZERO::InputKey::GAMEPAD_LEFT_THUMB_X, ZERO::InputAction {
                    .actionName = "strafe",
                    .scale = 1.0f,
                });
                _inputManager->MapInputToAction(ZERO::InputKey::GAMEPAD_LEFT_THUMB_Y, ZERO::InputAction {
                    .actionName = "move_forward",
                    .scale = 1.0f,
                });

                _inputManager->RegisterActionCallback("strafe", ZERO::InputManager::ActionCallback {
                    .Ref = "CppGame",
                    .Func = [](ZERO::InputSource source, int sourceIndex, float value) {
                        std::string direction { "NONE" };
                        if (value > 0.f) {
                            direction = "RIGHT";
                        }
                        if (value < 0.f) {
                            direction = "LEFT";
                        }
                        printf("strafing: %s | %f \n", direction.c_str(), value);
                        return true;
                    },
                });

                _inputManager->RegisterActionCallback("move_forward", ZERO::InputManager::ActionCallback {
                    .Ref = "CppGame",
                    .Func = [](ZERO::InputSource source, int sourceIndex, float value) {
                        std::string direction { "NONE" };
                        if (value > 0.f) {
                            direction = "UP";
                        }
                        if (value < 0.f) {
                            direction = "DOWN";
                        }
                        printf("moving: %s | %f \n", direction.c_str(), value);
                        return true;
                    },
                });

                _inputManager->MapInputToAction(ZERO::InputKey::MOUSE_LEFT, ZERO::InputAction {
                    .actionName = "click",
                });

                _inputManager->RegisterActionCallback("click", ZERO::InputManager::ActionCallback {
                    .Ref = "CppGame",
                    .Func = [this](ZERO::InputSource source, int sourceIndex, float value) {
                        handleClick(value);
                        return true;
                    },
                });
            }
        }


    protected:
        void Update(float deltaTime) override {
            // std::cout << "Update" << std::endl;
        }

    private:
        void handleClick(float value) {
            printf("clicked %f \n", value);
        }
        ZERO::InputManager *_inputManager { nullptr };
};

// Runtime
ZERO::Game* ZERO::CreateGame() {
    return new CppGame("Cpp Game");
}
