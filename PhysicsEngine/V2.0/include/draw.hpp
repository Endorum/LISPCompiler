#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "vec.hpp"

void drawCircle(SDL_Renderer* renderer,vec2 center, double radius, double angle){
    int x = 0;
    int y = radius;    
    int d = 1 - radius;

    std::vector<SDL_Point> points;
    
    while(x <= y){
        // Draw pixels in all eight octants
        points.push_back({static_cast<int>(center.x + x), static_cast<int>(center.y + y)});
        points.push_back({static_cast<int>(center.x - x), static_cast<int>(center.y + y)});
        points.push_back({static_cast<int>(center.x + x), static_cast<int>(center.y - y)});
        points.push_back({static_cast<int>(center.x - x), static_cast<int>(center.y - y)});
        points.push_back({static_cast<int>(center.x + y), static_cast<int>(center.y + x)});
        points.push_back({static_cast<int>(center.x - y), static_cast<int>(center.y + x)});
        points.push_back({static_cast<int>(center.x + y), static_cast<int>(center.y - x)});
        points.push_back({static_cast<int>(center.x - y), static_cast<int>(center.y - x)});

        // Update decision parameter
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }

    SDL_RenderDrawLine(renderer,center.x, center.y, center.x + radius*cos(angle), center.y + radius*sin(angle));

    SDL_RenderDrawPoints(renderer, points.data(), points.size());
}

void drawRect(SDL_Renderer* renderer, vec2 center, vec2 A, vec2 B, vec2 C, vec2 D){
    std::vector<SDL_Point> points;

    SDL_RenderDrawPoint(renderer, center.x, center.y);
    

    points.push_back({static_cast<int>(A.x), static_cast<int>(A.y)});
    points.push_back({static_cast<int>(B.x), static_cast<int>(B.y)});
    points.push_back({static_cast<int>(C.x), static_cast<int>(C.y)});
    points.push_back({static_cast<int>(D.x), static_cast<int>(D.y)});
    points.push_back({static_cast<int>(A.x), static_cast<int>(A.y)});

    SDL_RenderDrawLines(renderer, points.data(), points.size());
    
}