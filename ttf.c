#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "pongfunc.h"

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

int main()
{
    if (TTF_Init() != 0)
    {
        logSDLError("TTF_Init");
        SDL_Quit();
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("GIVE IT UP NOW", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (!win)
    {
        logSDLError("Window");
        SDL_Quit();
        return 1;
    }
    /* renderer sets up the graphics hardware */
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

    if (!rend)
    {
        logSDLError("Renderer");
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    SDL_Color color = {168, 115, 168, 255};
    
    SDL_Texture * image = renderText("THERES NOTHING LEFT TO SAY NOW", "../Desktop/ttf's/Spoops.ttf", color, 25, rend);
    if (image == 0)
    {
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    int iW, iH;
    SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
    int x = (WINDOW_WIDTH - iW) / 2;
    int y = (WINDOW_HEIGHT - iH) / 2;
    SDL_Rect d;
    d.x = x;
    d.y = y;
    d.w = iW;
    d.h = iH;

    SDL_RenderClear(rend);
    SDL_RenderCopy(rend, image, NULL, &d);
    SDL_RenderPresent(rend);
    SDL_Delay(10000);
}

