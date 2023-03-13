#include "raylib.h"
#include "./include/raylib-cpp.hpp"
#include "screen.hpp"
#include "game.hpp"
#include <iostream>

int main()
{
    Game game;
    game.initialize();
    game.runLoop();
    game.shutdown();

    return 0;
}