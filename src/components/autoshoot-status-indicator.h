#ifndef AUTOSHOOT_STATUS_INDICATOR_H
#define AUTOSHOOT_STATUS_INDICATOR_H

#include "SDL_ttf.h"
#include <stdbool.h>
#include <string.h>

void InitAutoShootIndicator(SDL_Renderer *ren, TTF_Font *font);
void ToggleAutoShoot(SDL_Renderer *ren, TTF_Font *font);
void DrawAutoShootIndicator(SDL_Renderer *ren);
void DestroyAutoShootIndicator();
bool GetAutoShoot();
void SetAutoShoot(bool value);

#endif
