

#include "Tile.hpp"
#include "Machine.hpp"

Tile::Tile(int x, int y) : is_empty(true), position(x,y), machine(nullptr), ress(nullptr) {}

bool Tile::PlaceMachine(Machine *new_machine) {
    if(is_empty) {
        machine = new_machine;
        is_empty = false;
        return true;
    }
    return false;
}

void Tile::RemoveMachine() {
    machine = nullptr;
    is_empty = true;
}

void Tile::UpdateTile() {
    if(machine != nullptr){
        machine->GetDirectNeighbors();
        machine->Update();
    }
}
