#pragma once

class Entity {
public:
    virtual ~Entity() = default;

    virtual void update() = 0;
    virtual void render() = 0;
};

