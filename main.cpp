#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <vector>

#include "RenderWindow.hpp"

const int SCREEN_RESOLUTION_X = 1280;
const int SCREEN_RESOLUTION_Y = 720;

void updateBoard(RenderWindow window, std::vector<int>& board, int width, int height) {
    std::vector<int> nextBoard(width * height, 0);

    int idx, idx_below, idx_below_left, idx_below_right;
    int state, state_below, state_below_left, state_below_right;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            idx = i * width + j;
            idx_below = (i + 1) * width + j;
            idx_below_left = (i + 1) * width + (j - 1);
            idx_below_right = (i + 1) * width + (j + 1);

            state = board[idx];
            if (state == 1) {
                if (i + 1 < height) {
                    state_below = board[idx_below];
                    if (state_below == 0) {
                        state = 0;
                        state_below = 1;
                        nextBoard[idx_below] = state_below;
                    }
                }

                nextBoard[idx] = state;
            }

            board[idx] = nextBoard[idx];
        }
    }
}

void handleMouseHeldEvent(std::vector<int>& board, int width, int scalingFactor) {
    int idx, mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (0 <= mouseX && mouseX < SCREEN_RESOLUTION_X && 0 <= mouseY &&
        mouseY < SCREEN_RESOLUTION_Y) {
        mouseX /= scalingFactor;
        mouseY /= scalingFactor;

        idx = mouseY * width + mouseX;
        board[idx] = 1;
    }
}

void drawBoard(RenderWindow window, std::vector<int>& board, int width, int height) {
    window.clearWindow();

    int idx, state;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            idx = i * width + j;
            state = board[idx];
            window.drawPixel(j, i, state);
        }
    }

    window.update();
}

int main(int argc, char* args[]) {
    const int frameRate = 30;
    const int scalingFactor = 10;

    const int scaledResolutionX = SCREEN_RESOLUTION_X / scalingFactor;
    const int scaledResolutionY = SCREEN_RESOLUTION_Y / scalingFactor;

    const char* pGameTitle = "Falling Sand";

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }

    if (!IMG_Init(IMG_INIT_PNG)) {
        printf("IMG could not initialize! SDL_Error: %s\n", SDL_GetError());
    }

    RenderWindow window(pGameTitle, SCREEN_RESOLUTION_X, SCREEN_RESOLUTION_Y);
    window.setRenderScale(scalingFactor, scalingFactor);

    std::vector<int> gameBoard(scaledResolutionX * scaledResolutionY, 0);
    bool gameRunning = true;
    bool mouseIsPressed = false;

    Uint64 startTickMs, endTickMs, durationMs;
    int timeStepMs = floor(1000 / frameRate);
    SDL_Event event;
    while (gameRunning) {
        startTickMs = SDL_GetTicks64();

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    gameRunning = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouseIsPressed = true;
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouseIsPressed = false;
                default:
                    break;
            }
        }

        if (mouseIsPressed) {
            handleMouseHeldEvent(gameBoard, scaledResolutionX, scalingFactor);
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
