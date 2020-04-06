/* attempt at PONG lel xd */

/* NOTES

   Most importantly stay organized. Maybe multiplayer?!. Points obtained by touching ends of window (alternating).

   Perhaps hotkeys to add special effects:
   Speed Boost
   Change direction (with lowered speed)
   Omnicontrol
   

 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "pongfunc.h"

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

#define SPEED (150)
#define ACCELERATION (10);

/* void logSDLError(char * msg) */
/* { */
/*     printf("%s error: %s\n", msg, SDL_GetError()); */
/* } */

/* SDL_Texture* loadTexture(SDL_Renderer re, char * path) */
/* { */
/*     /\* avoids dangling pointers by initializing null *\/ */
/*     SDL_Texture *texture = NULL; */

/*     SDL_Surface *loadedImage = IMG_Load(path); */

/*     if (loadedImage != 0) */
/*     { */
/*         texture = SDL_CreateTextureFromSurface(re, loadedImage); */
/*         SDL_FreeSurface(loadedImage); */

/*         if (texture == 0) */
/*         { */
/*             logSDLError("Texture"); */
/*         } */
/*     }  */
/*     else */
/*     { */
/*         logSDLError("Image load"); */
/*     } */
/*     return texture; */
/* } */

/* void renderTexture(SDL_Texture* tex, SDL_Renderer *r, int x, int y) */
/* { */
/*     SDL_Rect = dst; */
/*     dst.x = x; */
/*     dst.y = y; */
/*     /\* Query the texture to get the width and height *\/ */
/*     SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h); */
/*     SDL_RenderCopy(r, tex, NULL, &dst); */
/* } */

int main(void)
{

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        logSDLError("Initialize");
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("PONG ON STEROIDS BABY",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (!win)
    {
        logSDLError("Window");
        SDL_Quit();
        return 1;
    }
    /* renderer sets up the graphics hardware */
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

    if (!rend)
    {
        logSDLError("Renderer");
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* tex = loadTexture(rend, "../Desktop/sprites/blue.png");
    SDL_Texture* text = loadTexture(rend, "../Desktop/sprites/blue.png"); 
    SDL_Texture* backg = loadTexture(rend, "../Desktop/sprites/white.jpg");
    SDL_Texture* goo = loadTexture(rend, "../Desktop/sprites/gameover.png");
    SDL_Texture* alt = loadTexture(rend, "../Desktop/sprites/altmode.png");

    /* SDL_Surface* altie = IMG_Load("../Desktop/sprites/altmode.png"); */
    /* SDL_SetColorKey(altie, SDL_TRUE, SDL_MapRGB(altie->format, 0xFF, 0xFF, 0xFF)); */
    /* SDL_Rect altRect = {0, 0, 150, 75}; */

    // struct to hold the position and size of the sprites (ball and paddle)
    SDL_Rect dest, ball;
    /* dimensions of texture */
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    SDL_QueryTexture(text, NULL, NULL, &ball.w, &ball.h);
    dest.w = 18;
    dest.h = 120;

    ball.w = 14;
    ball.h = 14;

    /* sprites spawn point */
    float x_pos = 0;
    float y_pos = 0;
    float y_vel = 0;

    /* SEED CITY! */
    srand ( time(NULL) );
    
    float ballX = (WINDOW_WIDTH - ball.w) - rand() % 100; 
    float ballY = (WINDOW_HEIGHT - ball.h) / 2 + 100 - rand() % 200; /* randomness to ball's spawn location */
    float ballXV = 200 - (rand() % 100);
    float ballYV = 200 - (rand() % 100);
    if (rand() & 0x1) ballXV = -ballXV;
    if (rand() & 0x1) ballYV = - ballYV;

    printf("%d, %d\n", (int) ballXV, (int) ballYV);
    /* keyboard inputs which are given */
    int up = 0;
    int down = 0;
    
    /* set to 1 once x button pressed */
    int closeME = 0; /* altcount = 0; */
    int lives = 3;

    /* animation loop */
    while (!closeME)
    {

        /* keyboard state (pointer value checked at state[]...) */
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        /* event processor */
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {

            case SDL_QUIT:
                closeME = 1;
                break;
            case SDL_KEYDOWN:
                if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])
                    up = 1;
                    
                if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])
                    down = 1;
                break;
            case SDL_KEYUP:
                if (!state[SDL_SCANCODE_W] || !state[SDL_SCANCODE_UP])
                    up = 0;
                if (!state[SDL_SCANCODE_S] || !state[SDL_SCANCODE_DOWN])
                    down = 0;
                break;
            }
            break;
        }

        /* velocity calc'ed */
        if (up && !down) y_vel += -ACCELERATION;
        if (down && !up) y_vel += ACCELERATION;

        /* updates positions */
        y_pos += y_vel / 60;

        ballX += ballXV / 60;
        ballY += ballYV / 60;

        /* boundary detection for paddle*/
        if (y_pos >= WINDOW_HEIGHT - dest.h)
        {
            y_pos = WINDOW_HEIGHT - dest.h;
            y_vel = -y_vel * 0.4;
        }
        if (y_pos <= 0)
        {
            y_pos = 0;
            y_vel = -y_vel * 0.4;
        }

        /* boundary detection for ball*/
        /* if (ballX <= 0) */
        /* { */
        /*     ballX = 0; */
        /*     ballXV = -ballXV; */
        /* } */
        if (ballY <= 0)
        {
            ballY = 0;
            ballYV = - ballYV;
        }
        if (ballX >= (WINDOW_WIDTH - ball.w))
        {
            ballX = WINDOW_WIDTH - ball.w;
            ballXV = -ballXV;
        }
        if (ballY >= (WINDOW_HEIGHT - ball.h))
        {
            ballY = WINDOW_HEIGHT - ball.h;
            ballYV = -ballYV;
        }
        
        /* collision detection between ball and paddle */
        if (ballX <= dest.w + 3)
        {
            if ((ballY <= y_pos && (y_pos - ballY) <= ball.h) || (ballY >= dest.y && (ballY - y_pos) <= dest.h)) /* ball hits right side of paddle */
            {
                ballXV = -ballXV;
                ballYV = y_vel + 30 - rand() % 60; /* generates randomness in speed, avoiding loops in gameplay */
            }
            else
            {
                if (lives == 0)
                {
                    closeME = 1;
                }
                lives--;
                ballX = (WINDOW_WIDTH - ball.w) - rand() % 100; 
                ballY = (WINDOW_HEIGHT - ball.h) / 2 + 100 - rand() % 200; /* randomness to ball's spawn location */
                ballXV = 200 - (rand() % 100);
                ballYV = 200 - (rand() % 100);
                if (rand() & 0x1) ballXV = -ballXV;
                if (rand() & 0x1) ballYV = - ballYV;
                dest.h -= 40;
            }
        }

        /* sets positions for the images */
        dest.x = (int) x_pos;
        dest.y = (int) y_pos;

        ball.x = (int) ballX;
        ball.y = (int) ballY;

        /* altRect.x = ball.x - (altRect.w - ball.w) / 2; */
        /* altRect.y = ball.y - altRect.h; */

        /* clears window */
        SDL_RenderClear(rend);

        /* draws image to window */
        SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
        SDL_Rect home;
        home.w = dest.w;
        home.h = WINDOW_HEIGHT;
        home.x = 0;
        home.y = 0;
        SDL_RenderCopy(rend, backg, NULL, NULL);
        SDL_RenderCopy(rend, tex, NULL, &dest);
        SDL_RenderCopy(rend, text, NULL, &ball);
        SDL_RenderDrawRect(rend, &home);
        /* alt mode? */
        /* altcount++; */
        /* if (altcount % 60 == 0 && rand() % 15 == 0) */
        /* { */
        /*     if (ballY < altRect.h) */
        /*     { */
        /*         altRect.x = ball.x - (altRect.w - ball.w) / 2; */
        /*         altRect.y = ball.y + ball.h; */
        /*     } */
        /*     else if (ballX < altRect.w) */
        /*     { */
        /*         altRect.x = ball.x + ball.w; */
        /*         altRect.y = ball.y - (altRect.h - ball.h) / 2;  */
        /*     } */
        /*     else if (ballX > WINDOW_WIDTH - altRect.w) */
        /*     { */
        /*         altRect.x = ball.x - altRect.w; */
        /*         altRect.y = ball.y - (altRect.h - ball.h) / 2; */
        /*     } */

        /*     SDL_RenderCopy(rend, alt, NULL, &altRect); */
        /*     SDL_RenderPresent(rend); */
        /*     SDL_Delay(2000); */
        /*     ballXV *= 3; */
        /*     ballYV *= 3; */
        /* } */

        SDL_RenderPresent(rend);

        /* 60 fps omfg it works */
        SDL_Delay(1000/60);
    }

    SDL_RenderClear(rend);
    SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
    SDL_RenderCopy(rend, goo, NULL, NULL);
    SDL_RenderPresent(rend);
    SDL_Delay (2000);

    /* cleans up everything */
    SDL_DestroyTexture(tex);
    SDL_DestroyTexture(text);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();

}
