#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <engine/service_locator.h>

#include <engine/platform/game.h>

int main(int argc, char** argv) {
    // Create the game
    auto* theGame = CreateGame();

    // Run the game
    theGame->Run();

    // Cnce it's done, delete the game
    delete theGame;
}
