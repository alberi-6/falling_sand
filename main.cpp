#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <random>
#include <vector>

#include "RenderWindow.hpp"

const int SCREEN_RESOLUTION_X = 1280;
const int SCREEN_RESOLUTION_Y = 720;

void cycleColor(SDL_Color& color) {
    Uint8 r = color.r;
    Uint8 g = color.g;
    Uint8 b = color.b;

    if (r != 255) {
        r++;
    } else if (g != 255) {
        g++;
    } else if (b != 255) {
        b++;
    } else {
        r = g = b = 0;
    }

    color.r = r;
    color.g = g;
    color.b = b;
}

struct pixel {
    int state;
};

std::vector<pixel> newBoard(int width, int height) {
    std::vector<pixel> board(width * height);
    for (int i = 0; i < board.size(); i++) {
        pixel pixel = {0};
        board[i] = pixel;
    }

    return board;
}

void updateBoard(RenderWindow window, std::vector<pixel>& board, int width, int height) {
    std::vector<pixel> nextBoard = newBoard(width, height);

    double direction;
    int idx, idxBelow, idxBelowLeft, idxBelowRight;
    int state, stateBelow, stateBelowLeft, stateBelowRight;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            idx = i * width + j;
            idxBelow = (i + 1) * width + j;
            idxBelowLeft = (i + 1) * width + (j - 1);
            idxBelowRight = (i + 1) * width + (j + 1);

            // random +-1
            direction = std::copysign(1, ((double)rand() / (RAND_MAX)) - 0.5);
            state = board[idx].state;
            if (state == 1) {
                if (i + 1 < height) {
                    stateBelow = board[idxBelow].state;
                    if (stateBelow == 0) {
                        state = 0;
                        stateBelow = 1;
                        nextBoard[idxBelow].state = stateBelow;
                    } else if (direction == -1 && j > 0) {
                        stateBelowLeft = board[idxBelowLeft].state;

                        if (stateBelowLeft == 0) {
                            state = 0;
                            stateBelowLeft = 1;
                            nextBoard[idxBelowLeft].state = stateBelowLeft;
                        }
                    } else if (direction == 1 && j < width - 1) {
                        stateBelowRight = board[idxBelowRight].state;

                        if (stateBelowRight == 0) {
                            state = 0;
                            stateBelowRight = 1;
                            nextBoard[idxBelowRight].state = stateBelowRight;
                        }
                    }
                }

                nextBoard[idx].state = state;
            }

            board[idx] = nextBoard[idx];
        }
    }
}

void handleMouseHeldEvent(std::vector<pixel>& board, int width, int scalingFactor) {
    int idx, mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (0 <= mouseX && mouseX < SCREEN_RESOLUTION_X && 0 <= mouseY &&
        mouseY < SCREEN_RESOLUTION_Y) {
        mouseX /= scalingFactor;
        mouseY /= scalingFactor;

        idx = mouseY * width + mouseX;
        board[idx].state = 1;
    }
}

void drawBoard(RenderWindow window, std::vector<pixel>& board, int width, int height) {
    window.clearWindow();

    int idx, state;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            idx = i * width + j;
            state = board[idx].state;
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

    std::vector<pixel> gameBoard = newBoard(scaledResolutionX, scaledResolutionY);
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
