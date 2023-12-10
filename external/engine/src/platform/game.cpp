#include <engine/service_locator.h>
#include "engine/platform/game.h"
#include "multiplatform_window.h"
#include "rendering/vulkan/vulkan_renderer.h"
#include "sdl_window.h"

namespace ZERO
{
    Game::Game(): Game("New Cpp Engine Game") {}

    Game::Game(std::string windowTitle): _title(std::move(windowTitle)), _running(true) {
        initializeServices();
    }

    Game::~Game() {
        shutdownServices();
    }

    void Game::Run() {
        // Run the application
        while(_running) {
            // Update the window
            if (ServiceLocator::GetWindow()->Update()) {
                _running = false;
                continue;
            }

            if (ServiceLocator::GetInputManager()) {
                ServiceLocator::GetInputManager()->processInput();
            }

            // Calculate delta time

            // Update game states
            Update(0.0f);

            // Update physics

            // Draw
            ServiceLocator::GetRenderer()->RenderFrame();
        }
    }

    void Game::initializeServices() {
        // provide input manager
        ServiceLocator::Provide(new InputManager());
        // provide a window
        // ServiceLocator::Provide(new MultiplatformWindow());
        ServiceLocator::Provide(new SDLWindow());
        // Open the window
        ServiceLocator::GetWindow()->OpenWindow({
            .title = _title,
            .width = 800,
            .height = 600
        });
        // Initialize input system

        // Initialize the renderer
        RendererSettings settings {
            .ApplicationName = _title,
        };

        ServiceLocator::Provide(new VulkanRenderer(), settings);

    }

    void Game::shutdownServices() {
        ServiceLocator::ShutdownServices();
    }
}
