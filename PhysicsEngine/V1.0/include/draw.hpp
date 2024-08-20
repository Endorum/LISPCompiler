#pragma once

#include <SDL2/SDL.h>
#include "vec2.hpp"

#include "RB2D.hpp"

// Function to draw a circle using SDL_RenderDrawPoint
void drawCircle(SDL_Renderer* renderer, Circle circle){

    double centerX = circle.center.x; 
    double centerY = circle.center.y; 
    double radius = circle.radius;
    
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - ((int)radius << 1);

    while (x >= y) {
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0) {
            x--;
            dx += 2;
            err += dx - ((int)radius << 1);
        }
    }
}

void drawRect(SDL_Renderer* renderer, Rect rect){

        rect.to_ABCD();

        SDL_RenderDrawPointF(renderer, rect.center.x, rect.center.y);
        
        SDL_RenderDrawLineF(renderer, 
            rect.abcd.A.x,
            rect.abcd.A.y,

            rect.abcd.B.x,
            rect.abcd.B.y);

        SDL_RenderDrawLineF(renderer, 
            rect.abcd.B.x,
            rect.abcd.B.y,

            rect.abcd.C.x,
            rect.abcd.C.y);
        
        SDL_RenderDrawLineF(renderer, 
            rect.abcd.C.x,
            rect.abcd.C.y,

            rect.abcd.D.x,
            rect.abcd.D.y);

        SDL_RenderDrawLineF(renderer, 
            rect.abcd.D.x,
            rect.abcd.D.y,

            rect.abcd.A.x,
            rect.abcd.A.y);

    }

void drawRigidBody2D(SDL_Renderer* renderer, RigidBody2D rb){
    switch(rb.shape.type){
        default:break;
        case ST_CIRC: drawCircle(renderer, rb.shape.circ); break;
        case ST_RECT: drawRect(renderer, rb.shape.rect); break;
    }
}