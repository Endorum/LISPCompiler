#include <iostream>
#include <SDL2/SDL.h>

#include "../include/RB2D.hpp"
#include "../include/draw.hpp"
#include "../include/simulation.hpp"

SDL_Window* window;
SDL_Renderer* renderer;

int main(int argc, char** argv){
    
    bool show = true;

    

    if(show){

        if(SDL_Init(SDL_INIT_EVERYTHING)){
            std::cout << "SDL init error: " << SDL_GetError() << std::endl;
            return -1;
        }

        // Create a window
        window = SDL_CreateWindow("SDL2 Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
        if (!window) {
            std::cout << "SDL create window error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return -1;
        }

        // Create a renderer
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            std::cout << "SDL create renderer error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return -1;
        }

    }

    // create objects and such

    
    
    
    RigidBody2D rect(
        Rect(vec2(100,150), 100, 150, 15)
        
    );

    // RigidBody2D circ(
    //     Circle(vec2(100,350), 100)
        
    // );

    Simulation sim(0.1);

    sim.bodies.push_back(rect);
    //sim.bodies.push_back(circ);

    sim.ext_forces.push_back(vec2(0,9.81));
    //sim.ext_forces.push_back(vec2(1,0));

    bool quit = false;
    SDL_Event event;
    uint32_t start_time, frame_time;
    float fps=1;

    start_time = SDL_GetTicks();
    
    //main loop
    while(!quit){


        // check for close window
        while (SDL_PollEvent(&event) != 0) {
            // User requests quit
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        // Clear screen with black color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // run physics
        sim.update();
        sim.drawAll(renderer);
        sim.deltaTime = 1/10.0;

        

        
        

        


        sim.bodies[0].shape.rect.to_ABCD();


        
        
        
        

        // Update screen
        SDL_RenderPresent(renderer);


        frame_time = SDL_GetTicks()-start_time;
        fps = (frame_time > 0) ? 1000.0f / frame_time : 0.0f;

    }

    if(show){
        // Clean up
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    return 0;
}

// g++ -std=c++11 -Wall -O0 -g -lsdl2 -Iinclude -Llib src/main.cpp -o build/debug/play