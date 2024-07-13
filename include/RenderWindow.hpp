#pragma once
#include <SDL.h>
#include <SDL_image.h>

class RenderWindow {
   public:
    RenderWindow(const char *pTitle, int pWidth, int pHeight);
    void cleanUp();

   private:
    SDL_Window *window;
    SDL_Renderer *renderer;
};