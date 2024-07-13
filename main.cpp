#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <vector>

#include "RenderWindow.hpp"

void update(RenderWindow window, std::vector<int>& board, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int state = board[i * width + j];

            if (state == 1) {
                int state_below = board[(i + 1) + j];
                if (state_below == 0) {
                    window.drawPixel(j, i, state);
                }
            }
        }
    }

    window.update();
}

void handleMouseClickedEvent() {}

void handleMouseMovingEvent() {}

void drawBoard(RenderWindow window, std::vector<int>& board, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int state = board[i * width + j];
            window.drawPixel(j, i, state);
        }
    }

    window.update();
}

int main(int argc, char* args[]) {
    const int frameRate = 30;
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

    std::vector<int> gameBoard(screenResolutionX * screenResolutionY, 0);
    drawBoard(window, gameBoard, scaledResolutionX, scaledResolutionY);
    gameBoard[5] = 1;

    bool gameRunning = true;

    SDL_Event event;

    Uint64 startTickMs, endTickMs, durationMs;
    int timeStepMs = floor(1000 / frameRate);
    while (gameRunning) {
        startTickMs = SDL_GetTicks64();

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    gameRunning = false;
                    break;
                case SDL_MOUSEMOTION:
                    handleMouseMovingEvent();
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    handleMouseClickedEvent();
                    break;
                default:
                    break;
            }
        }

        endTickMs = SDL_GetTicks64();
        durationMs = endTickMs - startTickMs;
        if (durationMs < timeStepMs) {
            SDL_Delay(timeStepMs - (int)durationMs);
        }

        // updateBoard(window, gameBoard, scaledResolutionX, scaledResolutionY);
    }

    window.cleanUp();
    SDL_Quit();

    return 0;
}
