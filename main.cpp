#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <vector>

#include "RenderWindow.hpp"

void updateBoard(RenderWindow window, std::vector<int>& board, int width, int height) {
    std::vector<int> nextBoard(width * height, 0);

    int state, state_below;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            state = board[i * width + j];

            if (state == 1) {
                if (i + 1 < height) {
                    state_below = board[(i + 1) + j];
                    if (state_below == 0) {
                        state = 0;
                        state_below = 1;
                        nextBoard[(i + 1) * width + j] = state_below;
                    }
                }

                nextBoard[i * width + j] = state;
            }

            board[i * width + j] = nextBoard[i * width + j];
        }
    }
}

void handleMouseClickedEvent(std::vector<int>& board, int width, int scalingFactor) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    mouseX /= scalingFactor;
    mouseY /= scalingFactor;
    board[mouseY * width + mouseX] = 1;
}

void handleMouseMovingEvent() {}

void drawBoard(RenderWindow window, std::vector<int>& board, int width, int height) {
    window.clearWindow();

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

    std::vector<int> gameBoard(scaledResolutionX * scaledResolutionY, 0);
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
                    handleMouseClickedEvent(gameBoard, scaledResolutionX, scalingFactor);
                    break;
                default:
                    break;
            }
        }

        updateBoard(window, gameBoard, scaledResolutionX, scaledResolutionY);
        drawBoard(window, gameBoard, scaledResolutionX, scaledResolutionY);

        endTickMs = SDL_GetTicks64();
        durationMs = endTickMs - startTickMs;
        if (durationMs < timeStepMs) {
            SDL_Delay(timeStepMs - (int)durationMs);
        }
    }

    window.cleanUp();
    SDL_Quit();

    return 0;
}
