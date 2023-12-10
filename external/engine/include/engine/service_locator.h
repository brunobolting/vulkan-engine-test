#pragma once

#include <memory>
#include <any>
#include <engine/platform/window.h>
#include <engine/rendering/renderer.h>
#include <engine/input/input_manager.h>

namespace ZERO
{
    class ServiceLocator
    {
        public:
            static inline Window* GetWindow() {
                return _window.get();
            };

            static inline Renderer* GetRenderer() {
                return _renderer.get();
            };

            static inline InputManager* GetInputManager() {
                return _inputManager.get();
            };

            static inline void Provide(Window* window) {
                if (_window != nullptr) {
                    return;
                }
                _window = std::unique_ptr<Window>(window);
            }

            static inline void Provide(Renderer* renderer, RendererSettings settings) {
                if (_renderer != nullptr) {
                    return;
                }
                _renderer = std::unique_ptr<Renderer>(renderer);
                _renderer->Init(settings);
            }

            static inline void Provide(InputManager* inputManager) {
                if (_inputManager != nullptr) {
                    return;
                }
                _inputManager = std::unique_ptr<InputManager>(inputManager);
            }

            static inline void ShutdownServices() {
                // Ensure we shut down in the correct order
                // Usually opposite order of initialized
                shutdownInputManager();
                shutdownRenderer();
                shutdownWindow();
            }
        private:
            static inline std::unique_ptr<Window> _window = nullptr;
            static inline std::unique_ptr<Renderer> _renderer = nullptr;
            static inline std::unique_ptr<InputManager> _inputManager = nullptr;

        private:
            static inline void shutdownWindow() {
                _window.reset();
                _window = nullptr;
            }
            static inline void shutdownRenderer() {
                if (!_renderer) {
                    return;
                }
                _renderer->Shutdown();
                _renderer = nullptr;
            }
            static inline void shutdownInputManager() {
                if (!_inputManager) {
                    return;
                }
                _inputManager.reset();
            }
    };
}
