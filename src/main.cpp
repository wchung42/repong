#include "screen.hpp"
#include "game.hpp"

int main()
{
    Game game;
    game.initialize();
    game.runLoop();
    game.shutdown();

    return 0;
}