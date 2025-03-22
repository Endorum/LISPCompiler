#include <iostream>

#include <SDL2/SDL.h>

#include "../include/float2.hpp"
#include "../include/RigidBody2D.hpp"
#include "../include/Simulation.hpp"

int init_sdl(SDL_Window** window, SDL_Renderer** renderer){
    if(SDL_Init(SDL_INIT_EVERYTHING)){
        std::cout << "SDL init error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create a window
    *window = SDL_CreateWindow("SDL2 Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!(*window)) {
        std::cout << "SDL create window error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create a renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!(*renderer)) {
        std::cout << "SDL create renderer error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return -1;
    }
    return 0;
}

void cleanup_sdl(SDL_Window** window, SDL_Renderer** renderer){
    SDL_DestroyRenderer(*renderer);
    SDL_DestroyWindow(*window);
    SDL_Quit();
}

int main() {



    SDL_Window* window;
    SDL_Renderer* renderer;
    init_sdl(&window, &renderer);

    Simulation sim(renderer, 0.1f);

    // auto* rect = new RigidBody2D(
    //     new Rectangle(float2(400,550), 600, 25, 2),
    //     1.0,
    //     false
    // );
    //
    // auto* circ = new RigidBody2D(
    //     new Circle(float2(150,200), 20, 15),
    //     1.0,
    //     true
    // );



    // sim.addBody(new RigidBody2D(
    //     new Circle(float2(401,200), 40, 35),
    //     10.0,
    //     true
    // ));

    // sim.addBody(new RigidBody2D(
    //     new Circle(float2(400,500), 30, 15),
    //     1.0,
    //     false
    // ));
    //

    sim.addBody(new RigidBody2D(
        new Rectangle(float2(401,200), 70, 25, 340),
        10.0,
        true
    ));
    //
    // sim.addBody(new RigidBody2D(
    //     new Circle(float2(400,500), 30, 15),
    //     1.0,
    //     false
    // ));


    //
    // sim.addBody(new RigidBody2D(
    //     new Rectangle(float2(180,500), 600, 25, 10),
    //     1.0,
    //     false
    // ));

    sim.addBody(new RigidBody2D(
        new Rectangle(float2(400,350), 600, 25, 10),
        100.0,
        false
    ));


    bool quit = false;
    SDL_Event event;


    int i=0;
    while(!quit){


        // Check for close window
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear screen with black color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        //sim.applyExtForce(float2(0,9.81));

        sim.bodies[0]->applyForce(float2(0,9.81));
        sim.bodies[1]->applyForce(float2(0,-1));

        // if(i++ > 200)
        //     sim.bodies[0]->applyForce(sim.bodies[0]->velocity * sim.bodies[0]->mass * -1, dynamic_cast<Rectangle*>(sim.bodies[0]->shape)->BR);


        sim.step();
        sim.draw();


        SDL_RenderPresent(renderer);

    }

    cleanup_sdl(&window, &renderer);

    sim.deleteAll();

    return 0;
}
