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

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

#define SPEED (150)
#define ACCELERATION (10);

int main(void)
{

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("initialize error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("PONG?",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (!win)
    {
        printf("window error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    /* renderer sets up the graphics hardware */
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

    if (!rend)
    {
        printf("renderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    /* creates surface from image, loads image into memory */
    SDL_Surface* surface = IMG_Load("../Desktop/sprites/blue.png");
    SDL_Surface* surface1 = IMG_Load("../Desktop/sprites/blue.png");
    SDL_Surface* surf = IMG_Load("../Desktop/sprites/blue.png");
    SDL_Surface* background = IMG_Load("../Desktop/sprites/white.jpg");
    SDL_Surface* go = IMG_Load("../Desktop/sprites/gameover.png");
    SDL_Surface* altie = IMG_Load("../Desktop/sprites/altmode.png");
    SDL_SetColorKey(altie, SDL_TRUE, SDL_MapRGB(altie->format, 0xFF, 0xFF, 0xFF));
    SDL_Rect altRect = {0, 0, 150, 75};

    if (!surface || !surf || !surface1 || !background || !go || !altie)
    {
        printf("surface error");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    /* creates texture from renderer and surface */
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_Texture* tex1 = SDL_CreateTextureFromSurface(rend, surface1);
    SDL_Texture* text = SDL_CreateTextureFromSurface(rend, surf);
    SDL_Texture* backg = SDL_CreateTextureFromSurface(rend, background);
    SDL_Texture* goo = SDL_CreateTextureFromSurface(rend, go);
    SDL_Texture* alt = SDL_CreateTextureFromSurface(rend, altie);
    SDL_FreeSurface(surface);   /* frees surface */
    SDL_FreeSurface(surface1);   /* frees surface */
    SDL_FreeSurface(surf);   /* frees surface */
    SDL_FreeSurface(background);   /* frees surface */
    SDL_FreeSurface(go);   /* frees surface */
    SDL_FreeSurface(altie);   /* frees surface */

    if (!tex || !text || !tex1 || !alt || !backg || !goo)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // struct to hold the position and size of the sprites (ball and paddle)
    SDL_Rect dest, ball, dest1;
    /* dimensions of texture */
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    SDL_QueryTexture(tex1, NULL, NULL, &dest1.w, &dest1.h);
    SDL_QueryTexture(text, NULL, NULL, &ball.w, &ball.h);
    dest.w = 18;
    dest.h = 120;
    dest1.w = 18;
    dest1.h = 120;

    ball.w = 14;
    ball.h = 14;

    /* sprites spawn point */
    float x_pos = 0;
    float y_pos = 0;
    float y_vel = 0;

    float x_pos1 = WINDOW_WIDTH - dest1.w;
    float y_pos1 = 0;
    float y_vel1 = 0;

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
    
    int up1 = 0;
    int down1 = 0;
    /* set to 1 once x button pressed */
    int closeME = 0, altcount = 0;
    int lives = 3, lives1 = 3;

    /* animation loop */
    while (!closeME)
    {

        /* event processor */
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            const Uint8 *state = SDL_GetKeyboardState(NULL);
            switch (event.type)
            {
            case SDL_QUIT:
                closeME = 1;
                break;
            case SDL_KEYDOWN:
                if (state[SDL_SCANCODE_UP])
                {
                    up = 1;
                }
                else if (state[SDL_SCANCODE_DOWN])
                {
                    down = 1;
                }
                
                if (state[SDL_SCANCODE_W])
                {
                    up1 = 1;
                }
                else if (state[SDL_SCANCODE_S])
                {
                    down1 = 1;
                }
            case SDL_KEYUP:
                if (state[SDL_SCANCODE_UP])
                {
                    up = 0;
                }
                else if (state[SDL_SCANCODE_DOWN])
                {
                    down = 0;
                }
            
                if (state[SDL_SCANCODE_W])
                {
                    up1 = 0;
                }
                else if (state[SDL_SCANCODE_S])
                {
                    down1 = 0;
                }
            }
        }

        /* velocity calc'ed */
        if (up && !down) y_vel += -ACCELERATION;
        if (down && !up) y_vel += ACCELERATION;

        if (up1 && !down1) y_vel1 += -ACCELERATION;
        if (down1 && !up1) y_vel1 += ACCELERATION;
        /* updates positions */
        y_pos += y_vel / 60;

        y_pos1 += y_vel1 / 60;

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
        /* paddle1 ? */
        if (y_pos1 >= WINDOW_HEIGHT - dest1.h)
        {
            y_pos1 = WINDOW_HEIGHT - dest1.h;
            y_vel1 = -y_vel1 * 0.4;
        }
        if (y_pos1 <= 0)
        {
            y_pos1 = 0;
            y_vel1 = -y_vel1 * 0.4;
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
        if (ballX >= (WINDOW_WIDTH - ball.w - dest1.w))
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
                ballX = (WINDOW_WIDTH - ball.w) / 2;
                ballY = (WINDOW_HEIGHT - ball.h) / 2;
                ballXV = 200 - (rand() % 100);
                ballYV = 200 - (rand() % 100);
                if (rand() & 0x1) ballXV = -ballXV;
                if (rand() & 0x1) ballYV = - ballYV;
                dest.h -= 40;
            }
        }

        if (ballX >= WINDOW_WIDTH - dest.w - 3)
        {
            if ((ballY <= y_pos1 && (y_pos1 - ballY) <= ball.h) || (ballY >= dest1.y && (ballY - y_pos1) <= dest1.h)) /* ball hits side of paddle */
            {
                ballXV = -ballXV;
                ballYV = y_vel1 + 30 - rand() % 60; /* generates randomness in speed, avoiding loops in gameplay */
            }
            else
            {
                if (lives1 == 0)
                {
                    closeME = 1;
                }
                lives1--;
                ballX = (WINDOW_WIDTH - ball.w) / 2;
                ballY = (WINDOW_HEIGHT - ball.h) / 2;
                ballXV = 200 - (rand() % 100);
                ballYV = 200 - (rand() % 100);
                if (rand() & 0x1) ballXV = -ballXV;
                if (rand() & 0x1) ballYV = - ballYV;
                dest1.h -= 40;
            }
        }
        /* sets positions for the images */
        dest.x = (int) x_pos;
        dest.y = (int) y_pos;

        dest1.x = (int) x_pos1;
        dest1.y = (int) y_pos1;

        ball.x = (int) ballX;
        ball.y = (int) ballY;

        altRect.x = ball.x - (altRect.w - ball.w) / 2;
        altRect.y = ball.y - altRect.h;

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
        SDL_RenderCopy(rend, tex1, NULL, &dest1);
        SDL_RenderCopy(rend, text, NULL, &ball);
        SDL_RenderDrawRect(rend, &home);
        home.x = WINDOW_WIDTH - dest.w;
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
