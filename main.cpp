#include <SDL.h>
#include <SDL_image.h>

#include <iostream>

#include "RenderWindow.hpp"

int main(int argc, char* args[]) {
    const char* pGameTitle = "Falling Sand";

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }

    if (!IMG_Init(IMG_INIT_PNG)) {
        printf("IMG could not initialize! SDL_Error: %s\n", SDL_GetError());
    }

    RenderWindow window(pGameTitle, 1280, 720);
    window.setRenderScale(10, 10);
    window.clearWindow();

    bool gameRunning = true;

    SDL_Event event;

    while (gameRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameRunning = false;
            }
        }
    }

    window.cleanUp();
    SDL_Quit();

    return 0;
}
