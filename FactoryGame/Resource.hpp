#pragma once

enum class ResourceType {

};

class Resource {
public:
    ResourceType type;
    int amount;

    Resource(ResourceType tyoe, int amount);
    ResourceType getType() const;
    int harvest(int quantity); // returns harvested amount not always equal to the requested
    void render();

};
