#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include "SDL_render.h"
#include "SDL_video.h"

void InitHealthbar(SDL_Renderer *ren, SDL_Window *win);
void SetHP(Uint32 newHP);
void DrawHealthbar(SDL_Renderer *ren);
Uint32 GetHP();

#endif
