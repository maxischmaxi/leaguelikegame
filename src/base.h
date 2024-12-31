#ifndef BASE_H
#define BASE_H

#include "SDL_render.h"
#include "SDL_video.h"

void DrawBlackScreen(SDL_Renderer *ren);
SDL_Renderer *InitRenderer(SDL_Window *win);
SDL_Texture *LoadIdleTexture(SDL_Renderer *renderer, SDL_Window *window);
SDL_Texture *LoadWalkTexture(SDL_Renderer *renderer, SDL_Window *window);
SDL_Texture *LoadHealthbarTexture(SDL_Renderer *renderer, SDL_Window *window);

#endif
