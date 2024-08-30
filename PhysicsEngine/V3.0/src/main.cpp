#include <iostream>

#include <SDL2/SDL.h>



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

    bool quit = false;
    SDL_Event event;

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


        SDL_RenderPresent(renderer);

    }

    cleanup_sdl(&window, &renderer);


    return 0;
}
