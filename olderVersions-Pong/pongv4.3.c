/* attempt at PONG lel xd */

/* NOTES

   Most importantly stay organized.

   Perhaps hotkeys to add special effects:
   Ghost
   Speed Boost
   Change direction (with lowered speed)
   Omnicontrol
   

 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "pongfunc.h"

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

#define SPEED (150)
#define ACCELERATION (10);

int main(void)
{

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        logSDLError("Initialize");
        return 1;
    }

    if (TTF_Init() != 0)
    {
        logSDLError("TTF_Init");
        SDL_Quit();
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
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

    if (!rend)
    {
        logSDLError("Renderer");
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    /* text for point system */
    SDL_Color color = {158, 115, 168, 255};
    SDL_Color color1 = {142, 190, 0, 255};
    SDL_Color black = {0,0,0,255};
    
    SDL_Texture * image = renderText("POINTS:", "../Desktop/ttf's/Spoopies.ttf", color, 15, rend);
    SDL_Texture * image1 = renderText("POINTS:", "../Desktop/ttf's/Spoopies.ttf", color1, 15, rend);
    if (image == 0 || image1 == 0)
    {
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    int iW, iH;
    SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
    SDL_Rect d;
    d.x = 18;
    d.y = 0;
    d.w = iW;
    d.h = iH;
    /* loading textures */
    SDL_Texture* tex = loadTexture(rend, "../Desktop/sprites/blue.png");
    SDL_Texture* tex1 = loadTexture(rend, "../Desktop/sprites/blue.png");
    SDL_Texture* text = loadTexture(rend, "../Desktop/sprites/blue.png"); 
    SDL_Texture* backg = loadTexture(rend, "../Desktop/sprites/white.jpg");
    SDL_Texture* goo = loadTexture(rend, "../Desktop/sprites/gameover.png");

    /* final scores */
    SDL_Texture * fnum;
    SDL_Texture * fnum1;

    // struct to hold the position and size of the sprites (ball and paddle)
    SDL_Rect dest1, dest, ball;
    SDL_Rect def;
    def.x = 0;
    def.y = 0;
    def.w = WINDOW_WIDTH;
    def.h = WINDOW_HEIGHT;
    /* dimensions of texture */
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
    
    float ballX = (WINDOW_WIDTH - ball.w) / 2 + 50 - rand() % 100; 
    float ballY = (WINDOW_HEIGHT - ball.h) / 2 + 50 - rand() % 100; /* randomness to ball's spawn location */
    float ballXV = 200 - (rand() % 100);
    float ballYV = 200 - (rand() % 100);
    if (rand() & 0x1) ballXV = -ballXV;
    if (rand() & 0x1) ballYV = - ballYV;

    /* printf("%d, %d\n", (int) ballXV, (int) ballYV); */
    /* keyboard inputs which are given */
    int up = 0;
    int down = 0;

    int up1 = 0;
    int down1 = 0;
    
    /* set to 1 once x button pressed */
    int closeME = 0; /* altcount = 0; */
    int lives = 3, lives1 = 3;
    int p = 0, p1 = 0, s = 1, s1 = 1;

    int fW, fH, fW1, fH1;


    /* animation loop */
    while (!closeME)
    {

        /* keyboard state (pointer value checked at state[]...) */
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        /* event processor */
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                closeME = 1;
            }
            break;
        }
        if (closeME == 1) break;
        
        /* possible multiple outputs? */
        up = 0;
        down = 0;
        if (state[SDL_SCANCODE_UP])
            up = 1;
        if (state[SDL_SCANCODE_DOWN])
            down = 1;

        up1 = 0;
        down1 = 0;
        if (state[SDL_SCANCODE_W])
            up1 = 1;
        if (state[SDL_SCANCODE_S])
            down1 = 1;

        /* velocity calc'ed */
        if (up && !down) y_vel += -ACCELERATION;
        if (down && !up) y_vel += ACCELERATION;
        if (up && down)
        {
            if (y_vel >= 0) y_vel += -ACCELERATION;
            if (y_vel < 0) y_vel += ACCELERATION;
        }

        if (up1 && !down1) y_vel1 += -ACCELERATION;
        if (down1 && !up1) y_vel1 += ACCELERATION;
        if (up1 && down1)
        {
            if (y_vel1 >= 0) y_vel1 += -ACCELERATION;
            if (y_vel1 < 0) y_vel1 += ACCELERATION;
        }
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
            if (s == 1)
            {
               p++; 
               s = 0;
            } 
        }
        if (y_pos <= 0)
        {
            y_pos = 0;
            y_vel = -y_vel * 0.4;
            if (s == 0)
            {
               p++; 
               s = 1;
            } 
        }

        /* boundary detection for paddle 1*/
        if (y_pos1 >= WINDOW_HEIGHT - dest1.h)
        {
            y_pos1 = WINDOW_HEIGHT - dest1.h;
            y_vel1 = -y_vel1 * 0.4;
            if (s1 == 1)
            {
                p1++;  
                s1 = 0;
            }
        }
        if (y_pos1 <= 0)
        {
            y_pos1 = 0;
            y_vel1 = -y_vel1 * 0.4;
            if (s1 == 0)
            {
              p1++;  
              s1 = 1;
            }
        }

        /* boundary detection for ball */
        if (ballY <= 0)
        {
            ballY = 0;
            ballYV = - ballYV;
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
                ballX = (WINDOW_WIDTH - ball.w) / 2 + 50 - rand() % 100; 
                ballY = (WINDOW_HEIGHT - ball.h) / 2 + 50 - rand() % 100; /* randomness to ball's spawn location */
                ballXV = 200 - (rand() % 100);
                ballYV = 200 - (rand() % 100);
                if (rand() & 0x1) ballXV = -ballXV;
                if (rand() & 0x1) ballYV = - ballYV;
                dest.h -= 40;
            }
        }

        /* collision detection between ball and paddle 1 */
        if (ballX >= WINDOW_WIDTH - dest1.w - ball.w - 3)
        {
            if ((ballY <= y_pos1 && (y_pos1 - ballY) <= ball.h) || (ballY >= dest1.y && (ballY - y_pos1) <= dest1.h)) /* ball hits left side of paddle */
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
                ballX = (WINDOW_WIDTH - ball.w) / 2 + 50 - rand() % 100; 
                ballY = (WINDOW_HEIGHT - ball.h) / 2 + 50 - rand() % 100; /* randomness to ball's spawn location */
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

        /* clears window */
        SDL_RenderClear(rend);

        /* draws image to window */
        SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
        SDL_Rect home;
        home.w = dest.w;
        home.h = WINDOW_HEIGHT;
        home.x = 0;
        home.y = 0;
        renderTexture(backg, rend, NULL, def);
        /* SDL_RenderCopy(rend, backg, NULL, NULL); */
        SDL_RenderCopy(rend, tex, NULL, &dest);
        SDL_SetRenderDrawColor(rend, 158, 115, 168, 255);
        SDL_RenderFillRect(rend, &dest);

        SDL_RenderCopy(rend, tex1, NULL, &dest1);
        SDL_SetRenderDrawColor(rend, 142, 190, 0, 255);
        SDL_RenderFillRect(rend, &dest1);

        SDL_RenderCopy(rend, text, NULL, &ball);

        SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
        SDL_RenderDrawRect(rend, &home);
        home.x = WINDOW_WIDTH - dest.w;
        SDL_RenderDrawRect(rend, &home);

        /* integer to string for point system */
        char * snum = malloc (4 * sizeof(char));
        char * snum1 = malloc (4 * sizeof(char));
        sprintf(snum, "%d", p);
        sprintf(snum1, "%d", p1);

        /* width/height of point numbers */
        int W, H;
        
        /* "POINTS" for p1 */
        d.x = home.w;
        d.w = iW;
        d.h = iH;
        SDL_RenderCopy(rend, image, NULL, &d);

        /* number of points of p1 */
        SDL_Texture * num = renderText(snum, "../Desktop/ttf's/Spoopies.ttf", black, 15, rend);
        SDL_QueryTexture(num, NULL, NULL, &W, &H);
        d.x = home.w + iW;
        d.w = W;
        d.h = H;
        SDL_RenderCopy(rend, num, NULL, &d);

        /* records final width/height of number */
        fW = W;
        fH = H;

        /* "POINTS" for p2 */
        d.x = WINDOW_WIDTH - home.w - iW - 40;
        d.w = iW;
        d.h = iH;
        SDL_RenderCopy(rend, image1, NULL, &d);


        /* number of points of p2 */
        SDL_Texture * num1 = renderText(snum1, "../Desktop/ttf's/Spoopies.ttf", black, 15, rend);
        SDL_QueryTexture(num1, NULL, NULL, &W, &H);
        d.x = WINDOW_WIDTH - home.w - 40;
        d.w = W;
        d.h = H;
        SDL_RenderCopy(rend, num1, NULL, &d);

        /* records final scores */
        fnum = num;
        fnum1 = num1;
        fW1 = W;
        fH1 = H;

        SDL_RenderPresent(rend);

        /* 60 fps omfg it works */
        SDL_Delay(1000/60);
    }

    SDL_RenderClear(rend);

    color.r = 255;
    color.g = 255;
    color.b = 0;

    SDL_Rect display;

    SDL_Texture * score = renderText("SCORE: ", "../Desktop/ttf's/Spoopies.ttf", color, 15, rend);
    SDL_QueryTexture (score, NULL, NULL, &d.w, &d.h);
    d.x = 0;
    d.y = 0;
    
    if (p > p1)
    {
        SDL_SetRenderDrawColor(rend, 158, 115, 168, 255);  
        SDL_RenderFillRect(rend, NULL);
        SDL_Texture * pOne = renderText("PLAYER 1 WINS", "../Desktop/ttf's/Spoopies.ttf", color, 50, rend);
        SDL_QueryTexture(pOne, NULL, NULL, &display.w, &display.h);
        display.x = (WINDOW_WIDTH - display.w) / 2;
        display.y = (WINDOW_HEIGHT - display.h) / 2;
        SDL_RenderCopy(rend, pOne, NULL, &display);

        /* render copies score in top left */
        SDL_RenderCopy(rend, score, NULL, &d);
        d.x = d.w;
        d.w = fW;
        d.h = fH;
        SDL_RenderCopy(rend, fnum, NULL, &d);

        SDL_RenderPresent(rend);
        SDL_Delay (5000);
        SDL_DestroyTexture(pOne);
    }
    else if (p1 > p)
    {
        SDL_SetRenderDrawColor(rend, 142, 190, 0, 255);  
        SDL_RenderFillRect(rend, NULL);
        SDL_Texture * pTwo = renderText("PLAYER 2 WINS", "../Desktop/ttf's/Spoopies.ttf", color, 50, rend);
        SDL_QueryTexture(pTwo, NULL, NULL, &display.w, &display.h);
        display.x = (WINDOW_WIDTH - display.w) / 2;
        display.y = (WINDOW_HEIGHT - display.h) / 2;
        SDL_RenderCopy(rend, pTwo, NULL, &display);

        /* render copies score in top left */
        SDL_RenderCopy(rend, score, NULL, &d);
        d.x = d.w;
        d.w = fW1;
        d.h = fH1;
        SDL_RenderCopy(rend, fnum1, NULL, &d);

        SDL_RenderPresent(rend);
        SDL_Delay (5000);
        SDL_DestroyTexture(pTwo);
    }
    else if (p1 == p)
    {
        SDL_SetRenderDrawColor(rend, 168, 67, 144, 255);  
        SDL_RenderFillRect(rend, NULL);
        SDL_Texture * Tie = renderText("TIE", "../Desktop/ttf's/Spoopies.ttf", color, 70, rend);
        SDL_QueryTexture(Tie, NULL, NULL, &display.w, &display.h);
        display.x = (WINDOW_WIDTH - display.w) / 2;
        display.y = (WINDOW_HEIGHT - display.h) / 2;
        SDL_RenderCopy(rend, Tie, NULL, &display);
        /* render copies score in top left */
        SDL_RenderCopy(rend, score, NULL, &d);
        d.x = d.w;
        d.w = fW;
        d.h = fH;
        SDL_RenderCopy(rend, fnum, NULL, &d);

        SDL_RenderPresent(rend);
        SDL_Delay (5000);
        SDL_DestroyTexture(Tie);
    }

    /* gameover screen */
    SDL_RenderClear(rend);
    SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
    SDL_RenderFillRect(rend, NULL);
    SDL_RenderCopy(rend, goo, NULL, NULL);
    SDL_RenderPresent(rend);
    SDL_Delay (2000);

    /* cleans up everything */
    SDL_DestroyTexture(tex);
    SDL_DestroyTexture(text);
    SDL_DestroyTexture(backg);
    SDL_DestroyTexture(goo);
    SDL_DestroyTexture(tex1);
    SDL_DestroyTexture(image);
    SDL_DestroyTexture(image1);
    SDL_DestroyTexture(fnum);
    SDL_DestroyTexture(fnum1);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();

}
