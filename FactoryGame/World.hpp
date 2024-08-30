#pragma once
#include <assert.h>
#include <vector>

#include "Tile.hpp"

class Machine;

class World {
public:

    std::vector<Tile> tiles;
    int width, height;

    World(int width, int height);

    int Index(int x, int y) const;
    Tile& GetTile(int x, int y);
    void PlaceMachine(Machine* machine);
    void RemoveMachine(const int x, const int y);
    void PlaceRessource(Resource* new_ress) {
        GetTile(new_ress->position.x,new_ress->position.y).PlaceRessource(new_ress);
    }
    void UpdateWorld();

};
