#include <SDL2/SDL.h>
#include <vector>

#include "RB2D.hpp"

#define f_g vec2(0,9.81)
#define s_G -9.81

class Simulation{
public:
    std::vector<RigidBody2D> bodies;

    std::vector<vec2> ext_forces;

    double deltaTime;

    Simulation(double dt) : deltaTime(dt) {}

    void update(){
        for(RigidBody2D& rb : bodies){
            // rb.apply_force()
            for(vec2& ext_f : ext_forces){
                rb.apply_force(ext_f, center_of_shape_Read(rb.shape));
            }
            rb.update(deltaTime);
            if(rb.shape.type == ST_RECT) rb.shape.rect.to_ABCD();
        }
    }

    void drawAll(SDL_Renderer* renderer){
        for(RigidBody2D& rb : bodies){
            drawRigidBody2D(renderer, rb);
        }
    }

};