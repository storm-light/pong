#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

void logSDLError(char * msg)
{
    printf("%s error: %s\n", msg, SDL_GetError());
}

SDL_Texture* loadTexture(SDL_Renderer* re, char * path)
{
    /* avoids dangling pointers by initializing null */
    SDL_Texture *texture = NULL;

    SDL_Surface *loadedImage = IMG_Load(path);

    if (loadedImage != 0)
    {
        texture = SDL_CreateTextureFromSurface(re, loadedImage);
        SDL_FreeSurface(loadedImage);

        if (texture == 0)
        {
            logSDLError("Texture");
        }
    } 
    else
    {
        logSDLError("Image load");
    }
    return texture;
}

void renderTexture(SDL_Texture* tex, SDL_Renderer *r, SDL_Rect * clip, SDL_Rect dst)
{
    if (clip != 0)
    {
        dst.w = clip->w;
        dst.h = clip->h;
    }

    SDL_RenderCopy(r, tex, clip, &dst);
}

SDL_Texture* renderText(char * message, char * fontFile, SDL_Color color, int fontSize, SDL_Renderer * rend)
{
    /* opens font */
    TTF_Font * font = TTF_OpenFont(fontFile, fontSize);
    if (font == 0)
    {
        logSDLError("TTF OPENfont");
        return NULL;
    }

    /* render surface first with TTF_RenderText then create a texture for the surface */
    SDL_Surface *surf = TTF_RenderText_Blended(font, message, color);
    if (surf == 0)
    {
        TTF_CloseFont(font);
        logSDLError("TTF_RenderText");
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(rend, surf);
	if (texture == NULL){
		logSDLError("CreateTexture");
	}

    /* frees stuff up */
    SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}
