#include <iostream>
#include <vector>
#include "World.hpp"
#include "Machine.hpp"
#include "Resource.hpp"

int main() {

    World world(100,100);


    world.PlaceMachine(new MiningDrill(5,4, &world));
    world.PlaceMachine(new Conveyor(5,5, &world,RIGHT));
    world.PlaceMachine(new Conveyor(6,5, &world,RIGHT));
    world.PlaceMachine(new Conveyor(7,5, &world,RIGHT));
    // world.PlaceMachine(new MiningDrill(5,6, &world));
    // world.PlaceMachine(new Furnace(5,5, &world));


    auto iron = new Resource(Vec2(5,4),RT_IRON_ORE,1000);
    //auto coal = new Resource(Vec2(5,6),RT_COAL_ORE,1000);
    world.PlaceRessource(iron);
    //world.PlaceRessource(coal);

    // for(int i=0;i<20;i++) {
    //     world.UpdateWorld();
    // }

    world.UpdateWorld();
    world.UpdateWorld();





    return 0;
}
