#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <random>
#include <vector>

#include "RenderWindow.hpp"

const int SCREEN_RESOLUTION_X = 1280;
const int SCREEN_RESOLUTION_Y = 720;
SDL_Color DRAW_COLOR = {50, 50, 50};

void cycleColor() {
    Uint8 r = DRAW_COLOR.r;
    Uint8 g = DRAW_COLOR.g;
    Uint8 b = DRAW_COLOR.b;

    if (r != 255) {
        r++;
    } else if (g != 255) {
        g++;
    } else if (b != 255) {
        b++;
    } else {
        r = g = b = 0;
    }

    DRAW_COLOR.r = r;
    DRAW_COLOR.g = g;
    DRAW_COLOR.b = b;
}

struct pixel {
    bool active;
    SDL_Color color;
};

std::vector<pixel> newBoard(int width, int height) {
    std::vector<pixel> board(width * height);
    for (int i = 0; i < board.size(); i++) {
        SDL_Color initColor = {0, 0, 0};
        pixel pixel = {false, initColor};
        board[i] = pixel;
    }

    return board;
}

void updateBoard(RenderWindow window, std::vector<pixel>& board, int width, int height) {
    std::vector<pixel> nextBoard = newBoard(width, height);

    double direction;
    int idx, idxBelow, idxBelowLeft, idxBelowRight;
    bool active, activeBelow, activeBelowLeft, activeBelowRight;
    SDL_Color color, colorBelow, colorBelowLeft, colorBelowRight;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            idx = i * width + j;
            idxBelow = (i + 1) * width + j;
            idxBelowLeft = (i + 1) * width + (j - 1);
            idxBelowRight = (i + 1) * width + (j + 1);

            // random +-1
            direction = std::copysign(1, ((double)rand() / (RAND_MAX)) - 0.5);
            active = board[idx].active;
            color = board[idx].color;
            if (active) {
                if (i + 1 < height) {
                    activeBelow = board[idxBelow].active;
                    if (!activeBelow) {
                        active = false;
                        activeBelow = true;
                        colorBelow = board[idx].color;
                        nextBoard[idxBelow].active = activeBelow;
                        nextBoard[idxBelow].color = colorBelow;
                    } else if (direction == -1 && j > 0) {
                        activeBelowLeft = board[idxBelowLeft].active;

                        if (!activeBelowLeft) {
                            active = false;
                            activeBelowLeft = true;
                            colorBelowLeft = board[idx].color;
                            nextBoard[idxBelowLeft].active = activeBelowLeft;
                            nextBoard[idxBelowLeft].color = colorBelowLeft;
                        }
                    } else if (direction == 1 && j < width - 1) {
                        activeBelowRight = board[idxBelowRight].active;

                        if (!activeBelowRight) {
                            active = false;
                            activeBelowRight = true;
                            colorBelowRight = board[idx].color;
                            nextBoard[idxBelowRight].active = activeBelowRight;
                            nextBoard[idxBelowRight].color = colorBelowRight;
                        }
                    }
                }

                nextBoard[idx].active = active;
                nextBoard[idx].color = color;
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
        board[idx].active = true;
        board[idx].color = DRAW_COLOR;
    }
    cycleColor();
}

void drawBoard(RenderWindow window, std::vector<pixel>& board, int width, int height) {
    window.clearWindow();

    int idx;
    SDL_Color color;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            idx = i * width + j;
            color = board[idx].color;
            window.drawPixel(j, i, color);
        }
    }

    window.update();
}

int main(int argc, char* args[]) {
    const int frameRate = 60;
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
