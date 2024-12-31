#ifndef BASE_H
#define BASE_H

#include "SDL_render.h"
#include "SDL_ttf.h"
#include "SDL_video.h"

void DrawBlackScreen(SDL_Renderer *ren);
SDL_Window *Init();
SDL_Renderer *InitRenderer(SDL_Window *win, float *scale);
SDL_Texture *LoadTileTexture(SDL_Renderer *renderer, SDL_Window *window);
SDL_Texture *LoadIdleTexture(SDL_Renderer *renderer, SDL_Window *window);
SDL_Texture *LoadWalkTexture(SDL_Renderer *renderer, SDL_Window *window);
void CalculateAccumulator(int *moving, float *appliedSpeed, float *accumulator,
                          unsigned int *surfaceIndex);
TTF_Font *LoadFont();

#endif
