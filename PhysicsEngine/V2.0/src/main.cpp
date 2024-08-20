#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#include <SDL2/SDL.h>

#include "../include/RB2D.hpp"
#include "../include/simulation.hpp"

const int TARGET_FPS = 60;
const int FRAME_DURATION_MS = 1000 / TARGET_FPS;

int init_sdl(SDL_Window** window, SDL_Renderer** renderer){
    if(SDL_Init(SDL_INIT_EVERYTHING)){
        std::cout << "SDL init error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create a window
    *window = SDL_CreateWindow("SDL2 Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "SDL create window error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create a renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
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

std::unique_ptr<RigidBody2D> createRectRB(vec2 center, double w, double h, double a=0.0, double m=1.0, bool ma=true){
    return  std::make_unique<RigidBody2D>(  
            std::make_unique<Rect>( center, w, h, a), 
            m,
            ma
    );
}

std::unique_ptr<RigidBody2D> createCircRB(vec2 center, double radius, double m=1.0, bool ma=true){
    return  std::make_unique<RigidBody2D>(  
            std::make_unique<Circle>( center, radius), 
            m,
            ma
    );
}



int main(int argc, char** argv){
    SDL_Window* window;
    SDL_Renderer* renderer;
    init_sdl(&window, &renderer);

    Simulation sim(0.001);

    //Rect shapeRect(vec2(100,100), 100, 150, 15);
    std::unique_ptr<RigidBody2D> rectBody   = createRectRB(vec2(200,400),750,50,45.0,1.0,false);
    std::unique_ptr<RigidBody2D> rectBody2  = createRectRB(vec2(600,400),750,50,180.0-45.0,1.0,false);

    std::unique_ptr<RigidBody2D> circBody  = createCircRB(vec2(410,250),40,2.0,true);
    std::unique_ptr<RigidBody2D> circBody2  = createCircRB(vec2(100,100),30,1.0,true);
    //std::unique_ptr<RigidBody2D> circ2Body  = createCircRB(vec2(400,400),50,1.0,false);
    // std::unique_ptr<RigidBody2D> rectBody2 = createRectRB(vec2(400,300),100,75,25.0,1.0,true);
    
    //circBody.get()->angular_velocity = 10.0;

    sim.bodies.push_back(std::move(rectBody));
    sim.bodies.push_back(std::move(rectBody2));
    sim.bodies.push_back(std::move(circBody));
    sim.bodies.push_back(std::move(circBody2));
    sim.renderer = renderer;

    bool quit = false;
    SDL_Event event;

    auto lastFrameTime = std::chrono::high_resolution_clock::now();

    // main loop
    while(!quit){
        auto frameStart = std::chrono::high_resolution_clock::now();
        
        // Check for close window
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear screen with black color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw color for objects is white
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // Calculate deltaTime
        auto currentFrameTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedTime = currentFrameTime - lastFrameTime;
        double deltaTime = elapsedTime.count(); // in seconds

        // Output deltaTime for debugging
        ///std::cout << "Delta Time: " << deltaTime << " seconds" << std::endl;

        // Apply gravity and update simulation
        //sim.apply_ext_force(vec2(0, 9.81)); // gravity now in every rb 
        
        sim.dt = 1.0/60.0;
        
        
        sim.step(); // Pass deltaTime to your simulation step
        

        sim.draw(renderer);

        SDL_RenderPresent(renderer);

        // Calculate frame duration and sleep if necessary
        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();
        if (frameDuration < FRAME_DURATION_MS) {
            std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_DURATION_MS - frameDuration));
        }

        lastFrameTime = std::chrono::high_resolution_clock::now();

    }

    cleanup_sdl(&window, &renderer);
    return 0;
}