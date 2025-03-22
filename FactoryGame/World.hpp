#pragma once


#include <vector>

#include "Entity.hpp"
#include "Tile.hpp"

class World {
public:
    std::vector<Tile> tiles;
    std::vector<Entity*> entities;

    World(int width, int height);
    Tile& getTile(int x, int y);
    void update();
};
