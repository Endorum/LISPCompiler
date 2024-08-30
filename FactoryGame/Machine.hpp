#pragma once
#include <assert.h>

#include "Item.hpp"
#include "Tile.hpp"

#include "Vec.hpp"


class World;
class Tile;

enum MachineType {
    NONE,
    MINING_DRILL,
    FURNACE,
    BELT
};

class Machine{
public:
    Vec2 position;
    World* world;
    std::vector<Tile*> neighbors;

    MachineType type;

    Machine(int x, int y, World* world);
    virtual ~Machine() = default;

    virtual bool is_miner() { return false; }

    std::vector<Tile*> GetDirectNeighbors();
    Tile GetTileBelow();
    // std::vector<Machine*> GetAllNeighbors();
    virtual void Update() = 0;
};

class Tile;

class MiningDrill : public Machine {
public:
    Item output;


    MiningDrill(int x, int y, World* world) : Machine(x,y,world){
        type = MINING_DRILL;
    }

    bool is_miner() override { return true; }

    void Update() override;
};

class Furnace : public Machine {
public:
    Item input_ore;
    Item input_fuel;
    Item output;

    int speed{1};

    Furnace(int x, int y, World* world) : Machine(x,y,world) {
        type = FURNACE;
    }

    std::vector<Machine*> NearMiners() {
        std::vector<Machine*> miners;
        for(auto& m : neighbors) {
            if(m->machine != nullptr && m->machine->is_miner()) {
                miners.push_back(m->machine);
            }
        }
        return miners;
    }

    void TransferFromMiner(Machine* machine) {
        assert(machine != nullptr);
        assert(machine->is_miner());
        auto miner = dynamic_cast<MiningDrill*>(machine);
        if(miner->output.quantity <= 0) return;
        if(miner->output.smeltable()) {
            int amount = miner->output.RemoveItems(1);
            input_ore.AddItems(miner->output.type, amount);
            return;
        }
        if(miner->output.fuel()) {
            int amount = miner->output.RemoveItems(1);
            input_fuel.AddItems(miner->output.type, amount);
            return;
        }
    }

    void smelt() {
        if(input_ore.quantity < 10 || input_fuel.quantity <= 0) return;
        auto res_type = IT_PLACE;
        switch (input_ore.type) {
            default: res_type = IT_PLACE; return;
            case IT_IRON_ORE: res_type = IT_IRON_PLATE;
            //...
        }
        input_ore.RemoveItems(10);
        input_fuel.RemoveItems(1);
        output.AddItems(res_type, 10);


    }

    void Update() override {
        auto minersNear = NearMiners();

        for(auto& m : minersNear) {
            TransferFromMiner(m);
        }

        smelt();


        printf("Furnace\n");
        printf("    ore input:  %s\n",input_ore.str().c_str());
        printf("    fuel input: %s\n",input_fuel.str().c_str());
        printf("    output:     %s\n",output.str().c_str());
    }
};

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
}Direction;

class Conveyor : public Machine {
public:
    Direction direction;
    Item onBelt;

    Conveyor(int x, int y, World* world, Direction dir) : Machine(x,y,world), direction(dir) {
        neighbors = GetDirectNeighbors();
        type = BELT;
    }

    std::vector<Machine*> NearMachines() {
        for(auto& b : neighbors) {
            std::vector<Machine*> machines;
            for(auto& m : neighbors) {
                if(m->machine != nullptr) {
                    machines.push_back(m->machine);
                }
            }
            return machines;
        }
    }

    /*
    auto miner = dynamic_cast<MiningDrill*>(machine);
    if(miner->output.quantity <= 0) return;
    if(miner->output.smeltable()) {
        int amount = miner->output.RemoveItems(1);
        input_ore.AddItems(miner->output.type, amount);
        return;
    }
    if(miner->output.fuel()) {
        int amount = miner->output.RemoveItems(1);
        input_fuel.AddItems(miner->output.type, amount);
        return;
    }
     */

    void Transfer(Machine* other) {
        assert(other != nullptr);
        if(onBelt.quantity >= 2) return;
        switch(other->type) {
            default:break;
            case MINING_DRILL: {
                auto miner = dynamic_cast<MiningDrill*>(other);
                int amount = miner->output.RemoveItems(1);
                onBelt.AddItems(miner->output.type, amount);
                break;
            }
            case FURNACE: {
                auto furnace = dynamic_cast<Furnace*>(other);
                int amount = furnace->output.RemoveItems(1);
                onBelt.AddItems(furnace->output.type, amount);
                break;
            }
            case BELT: {
                auto belt = dynamic_cast<Conveyor*>(other);
                int amount = belt->onBelt.RemoveItems(1);
                onBelt.AddItems(belt->onBelt.type, amount);
                break;
            }
        }
    }

    void Update() override {
        auto nearMachines = NearMachines();
        for(auto& b : nearMachines) {
            Transfer(b);
        }
        printf("Belt\n");
        printf("    on belt:  %s\n",onBelt.str().c_str());
    }
};