#include "RenderWindow.hpp"

#include <SDL.h>
#include <SDL_image.h>

RenderWindow::RenderWindow(const char *p_Title, int p_Width, int p_Height)
    : window(NULL), renderer(NULL) {
    window = SDL_CreateWindow(p_Title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_Width,
                              p_Height, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void RenderWindow::cleanUp() { SDL_DestroyWindow(window); }