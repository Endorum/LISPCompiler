


#include "Machine.hpp"
#include "World.hpp"
#include "Tile.hpp"
#include "Resource.hpp"

class Tile;

Machine::Machine(int x, int y, World* world) : position(x,y), world(world) {
    neighbors = GetDirectNeighbors();
}

std::vector<Tile*> Machine::GetDirectNeighbors() {
    std::vector<Tile*> neighbors;
    std::vector<Vec2> directions = {
        Vec2(0, 1), Vec2(0, -1), Vec2(1, 0), Vec2(-1, 0)
    };

    for (const auto& dir : directions) {
        int nx = position.x + dir.x;
        int ny = position.y + dir.y;

        // Check boundaries
        if (nx >= 0 && ny >= 0 && nx < world->width && ny < world->height) {
            Tile& neighbor_tile = world->GetTile(nx, ny);
            neighbors.push_back(&neighbor_tile);
        }
    }
    return neighbors;
}

Tile Machine::GetTileBelow() {
    return world->GetTile(position.x,position.y);
}

ItemType ItemTypeFromRessource(Resource& ress) {
    switch (ress.type) {
        default: return IT_PLACE;
        case RT_IRON_ORE: return IT_IRON_ORE;
        case RT_COAL_ORE: return IT_COAL_ORE;
    }
}

void MiningDrill::Update() {
    auto tileBelow = GetTileBelow();

    if(tileBelow.ress != nullptr) {
        auto ressource = tileBelow.ress;
        if(ressource->type != RT_PLACE_HOLDER) {
            auto r_type = ItemTypeFromRessource(*ressource);
            auto r_amount = (ressource->amount > 0) ? 1 : 0;
            output.type = r_type;
            output.quantity += r_amount;
        }
    }

    // printf("MiningDrill at %d %d:\n",position.x,position.y);
    // printf("    output: %s\n",output.str().c_str());
}


