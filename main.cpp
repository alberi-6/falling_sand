#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <vector>

#include "RenderWindow.hpp"

std::vector<int> initBoard(int width, int height) {
    std::vector<int> board(width * height, 0);
    return board;
}

int main(int argc, char* args[]) {
    const int screenResolutionX = 1280;
    const int screenResolutionY = 720;
    const int scalingFactor = 10;

    const int scaledResolutionX = screenResolutionX / scalingFactor;
    const int scaledResolutionY = screenResolutionY / scalingFactor;

    const char* pGameTitle = "Falling Sand";

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }

    if (!IMG_Init(IMG_INIT_PNG)) {
        printf("IMG could not initialize! SDL_Error: %s\n", SDL_GetError());
    }

    RenderWindow window(pGameTitle, screenResolutionX, screenResolutionY);
    window.setRenderScale(scalingFactor, scalingFactor);
    window.clearWindow();

    std::vector<int> gameBoard = initBoard(scaledResolutionX, scaledResolutionY);

    std::cout << gameBoard.size() << std::endl;

    bool gameRunning = true;

    SDL_Event event;
    while (gameRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    gameRunning = false;
                    break;

                case SDL_MOUSEMOTION:
                    std::cout << "mouse moving" << std::endl;
                case SDL_MOUSEBUTTONDOWN:
                    std::cout << "mouse pressed" << std::endl;
                default:
                    break;
            }
        }
    }

    window.cleanUp();
    SDL_Quit();

    return 0;
}
