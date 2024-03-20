#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "main.hpp"

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 700;

SDL_Texture* menuBackground = nullptr;
struct sizebutton
{
    int x;
    int y;
    int w;
    int h;
}button;

void renderMenu(){
    button.w = WINDOW_WIDTH/5;
    button.h = WINDOW_HEIGHT/5;
    button.x = (WINDOW_WIDTH-button.w)/2;
    button.y = (WINDOW_HEIGHT-button.h)/2;
    SDL_Rect rectButton = {button.x, button.y, button.w, button.h};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rectButton);
}
