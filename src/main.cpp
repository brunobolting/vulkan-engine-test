#include <engine/platform/entry_point.h>
#include <engine/platform/game.h>

class CppGame: public Game
{
    public:
        explicit CppGame(std::string title): Game(std::move(title)) {
            std::cout << "Hello, Cpp Game" << std::endl;
        }

    protected:
        void Update(float deltaTime) override {
            // std::cout << "Update" << std::endl;
        }
};

// Runtime
Game* CreateGame() {
    return new CppGame("Cpp Game");
}
