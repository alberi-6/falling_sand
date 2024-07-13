#pragma once
#include <SDL.h>
#include <SDL_image.h>

class RenderWindow {
   public:
    int width;
    int height;

    RenderWindow(const char *pTitle, int width, int height);
    void cleanUp();

    void clearWindow();
    void setRenderScale(int scaleX, int scaleY);
    void drawPixel(int x, int y, int state);
    void update();

   private:
    SDL_Window *window;
    SDL_Renderer *renderer;
};