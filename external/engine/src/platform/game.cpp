#include "engine/platform/game.h"
#include <engine/service_locator.h>
#include "multiplatform_window.h"

Game::Game(): Game("New Cpp Engine Game") {}

Game::Game(std::string windowTitle): _title(std::move(windowTitle)), _running(true) {
    initializeServices();
}

Game::~Game() {
    shutdownServices();
}

void Game::Run() {
    // Open the window
    ServiceLocator::GetWindow()->OpenWindow({
        .title = _title,
        .width = 800,
        .height = 600
    });
    // Run the application
    while(_running) {
        // Update the window
        if (ServiceLocator::GetWindow()->Update()) {
            _running = false;
            continue;
        }

        // Calculate delta time

        // Update game states
        Update(0.0f);

        // Update physics

        // Draw
    }
}

void Game::initializeServices() {
    // provide a window
    ServiceLocator::Provide(new MultiplatformWindow());

    // Initialize input system

    // Initialize the renderer

}

void Game::shutdownServices() {
    ServiceLocator::ShutdownServices();
}
