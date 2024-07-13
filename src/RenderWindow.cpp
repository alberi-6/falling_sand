#include "RenderWindow.hpp"

#include <SDL.h>
#include <SDL_image.h>

RenderWindow::RenderWindow(const char *pTitle, int width, int height)
    : window(NULL), renderer(NULL) {
    this->window = SDL_CreateWindow(pTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    width, height, SDL_WINDOW_SHOWN);

    if (this->window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }

    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(this->renderer, width, height);

    this->width = width;
    this->height = height;
}

void RenderWindow::cleanUp() { SDL_DestroyWindow(window); }

void RenderWindow::clearWindow() {
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->renderer);
}

void RenderWindow::setRenderScale(int scaleX, int scaleY) {
    SDL_RenderSetScale(this->renderer, scaleX, scaleY);
}

void RenderWindow::drawPixel(int x, int y) {
    SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(this->renderer, x, y);
}

void RenderWindow::update() { SDL_RenderPresent(this->renderer); }
