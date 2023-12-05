#include <iostream>
#include <engine/engine.h>
#include <glm/glm.hpp>
#include <engine/service_locator.h>

int main(int argc, char** argv) {
    std::cout << "Hello, world!" << std::endl;
    Engine::Init();
    ServiceLocator::GetWindow()->OpenWindow();
    while(!ServiceLocator::GetWindow()->Update()) {
    }
}
