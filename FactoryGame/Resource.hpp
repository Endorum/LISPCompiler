#pragma once
#include "Vec.hpp"


enum RessourceType {
    RT_PLACE_HOLDER,
    RT_IRON_ORE,
    RT_COAL_ORE
};

class Resource{
public:
    Vec2 position;
    RessourceType type;
    int amount;
    Resource(Vec2 pos, RessourceType type=RT_PLACE_HOLDER, int amount=0) : position(pos), type(type), amount(amount) {}
};