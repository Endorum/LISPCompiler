#pragma once


class Machine;
class Resource;

enum class TileType {

};

class Tile {
public:
    TileType type;
    Resource* resource;
    Machine* machine;

    Tile(TileType type);
    bool isBuildable() const;
    Resource* getResource();
    void setMachine(Machine* machine);

};
