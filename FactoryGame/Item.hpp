#pragma once
#include <string>
#include <vector>


typedef enum {
    IT_PLACE,
    IT_IRON_ORE,
    IT_IRON_PLATE,
    IT_COAL_ORE,
}ItemType;

class Item {
public:
    ItemType type;
    int quantity;
    std::string name;
    std::vector<Item> items;

    bool smeltable() {
        switch (type) {
            default: return false;
            case IT_IRON_ORE: return true;
        }
    }
    bool fuel() {
        switch (type) {
            default: return false;
            case IT_COAL_ORE: return true;
        }
    }

    Item(ItemType type=IT_PLACE, int quantity=0)
        :   type(type),
            quantity(quantity),
            name(GetItemName(type))
    {}

    std::string GetItemName(ItemType type) {
        switch (type) {
            case IT_IRON_ORE: return "Iron-ore";
            case IT_IRON_PLATE: return "Iron-plate";
            case IT_COAL_ORE: return "Coal-ore";
            default: return "Empty";
        }
    }

    bool IsEmpty() {
        return quantity <= 0;
    }

    void UseItem() {
        if(!IsEmpty()) {
            quantity--;
        }
    }

    void AddQuantity() {
        quantity++;
    }

    void AddItems(ItemType new_type, int amount) {
        if(type == IT_PLACE) {
            type = new_type;
            quantity = amount;
            return;
        }
        if(type != new_type) {
            printf("error\n");
            return;
        }
        quantity += amount;
    }

    int RemoveItems(int amount) {
        if(type == IT_PLACE) {
            return 0;
        }
        if(quantity <= 0) {
            quantity = 0;
            type = IT_PLACE;
            return 0;
        }
        if(amount > quantity) {
            return quantity - amount;
        }
        quantity -= amount;
        return amount;
    }



    std::string str() {
        return GetItemName(type) + " : " + std::to_string(quantity);
    }

};