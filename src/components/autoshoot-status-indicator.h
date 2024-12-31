#ifndef AUTOSHOOT_STATUS_INDICATOR_H
#define AUTOSHOOT_STATUS_INDICATOR_H

#include "SDL_ttf.h"
#include <string.h>

void InitAutoShootIndicator(SDL_Renderer *ren, TTF_Font *font, float *scale);
void ToggleAutoShoot(SDL_Renderer *ren, TTF_Font *font, float *scale);
void DrawAutoShootIndicator(SDL_Renderer *ren);
void DestroyAutoShootIndicator();

#endif
