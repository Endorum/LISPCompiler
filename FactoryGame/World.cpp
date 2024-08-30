
#include "World.hpp"
#include "Machine.hpp"

World::World(int width, int height) : width(width), height(height) {
    tiles.reserve(width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            tiles.emplace_back(x, y);
        }
    }
}




int World::Index(int x, int y) const {
    return y * width + x;
}

Tile& World::GetTile(int x, int y) {
    return tiles[Index(x, y)];
}

void World::PlaceMachine(Machine* machine) {
    assert(machine);
    GetTile(machine->position.x,machine->position.y).PlaceMachine(machine);
}

void World::RemoveMachine(const int x, const int y) {
    GetTile(x,y).RemoveMachine();
}

void PlaceRessource(Resource* new_ress) {

}

void World::UpdateWorld() {
    for(auto& tile : tiles) {
        tile.UpdateTile();
    }
}