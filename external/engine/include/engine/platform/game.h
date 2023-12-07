#pragma once

#include <string>

namespace ZERO
{
    class Game
    {
        public:
            Game();
            explicit Game(std::string windowTitle);
            ~Game();
            void Run();

        protected:
            virtual void Update(float deltaTime) {};
            virtual void PhysicsUpdate(float deltaTime) {};

        private:
            void initializeServices();
            void shutdownServices();

        public:

        private:
            std::string _title;
            bool _running;
    };

    Game* CreateGame();
}
