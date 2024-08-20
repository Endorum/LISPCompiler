#pragma once

#include <SDL2/SDL.h>
#include <memory>

#include "shape.hpp"
#include "draw.hpp"

class RigidBody2D{
public:
    std::unique_ptr<Shape> shape;
    vec2 velocity;
    vec2 acceleration;
    vec2 force;

    double mass;
    double MOI;

    double angular_velocity;
    double angular_acceleration;
    double torque;

    bool moveable;

    RigidBody2D(std::unique_ptr<Shape> s, double m=1.0, bool ma=true) 
        : shape(std::move(s)), mass(m), moveable(ma) {
            velocity = vec2();
            acceleration = vec2();
            force = vec2();
            torque = 0.0;
            // center MOI as a start
            MOI = shape->get_MOI(mass,shape->center);

        }
    
    double area() const {
        return shape->area();
    }

    void draw(SDL_Renderer* renderer) const {
        shape->draw(renderer);
    }

    void apply_force(vec2 f, vec2 point){
        // apply linear force
        force += f;

        // calculate torque and apply it
        vec2 r = point - shape->center; // vector from center to point of application
        double torque_applied = r.x * f.y - r.y * f.x; // Torque is the cross product (r x f)
        torque += torque_applied;
    }

    // applied at the center no rotation
    void apply_force(vec2 f){
        apply_force(f,shape->center);
    }

    vec2 compute_forces(){
        return vec2();
    }

    

           

    // void step(double dt, Simulation& sim){
        

    // }

    void update(double dt){
        if(moveable == true){
            // linear updates
            acceleration = force / mass;    // a = F/m
            velocity += acceleration * dt;  // v = v0 + a * dt
            shape->center += velocity * dt; // p = p0 + v * dt

            // angular updates
            angular_acceleration = torque / MOI;            // ɑ = T / I
            angular_velocity += angular_acceleration * dt;  // ω = ω0 + ɑ * dt
            shape->angle += angular_velocity * dt;          // θ = θ0 + ω * dt
        }
        

        force = vec2(0.0,0.0);
        torque = 0.0;

    }

};