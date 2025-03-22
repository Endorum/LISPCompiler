#include <iostream>

#include "Game.hpp"

int main() {

    Game game;
    World world(100, 100);
    game.world = &world;

    return 0;
}
