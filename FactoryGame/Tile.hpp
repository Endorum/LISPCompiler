#pragma once


#include "Vec.hpp"
#include "Resource.hpp"

class Machine;



class Tile {
public:
    bool is_empty;
    Vec2 position;

    Machine* machine;
    Resource* ress;


    Tile(int x, int y);

    bool PlaceMachine(Machine* new_machine);



    void RemoveMachine();

    void PlaceRessource(Resource* new_ress) {
        ress = new_ress;
    }

    void UpdateTile();
};
