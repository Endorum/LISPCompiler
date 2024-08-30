#pragma once

struct Vec2 {
    int x,y;

    Vec2(int x=0, int y=0) : x(x), y(y) {}

    bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vec2& other) const {
        return !(*this == other);
    }
};