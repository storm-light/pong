void logSDLError(char * msg);

SDL_Texture * loadTexture(SDL_Renderer* re, char * path);

void renderTexture(SDL_Texture* tex, SDL_Renderer *r, SDL_Rect * clip, SDL_Rect dst);

SDL_Texture* renderText(char * message, char * fontFile, SDL_Color color, int fontSize, SDL_Renderer * rend);
