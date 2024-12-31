#ifndef PLAYER_H
#define PLAYER_H

#include "../vector.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"

Uint32 GetSpriteIndex(int dirX, int dirY, int lastDirX, int lastDirY);
SDL_Rect GetPlayerHitbox();

void InitPlayer(SDL_Renderer *ren, SDL_Window *win);
void DrawPlayer(SDL_Renderer *ren, float deltaTime);
void DestroyPlayer();
void SetMoving(int value);
Vector2 GetPlayerPosition();
float GetSpeed();
Uint8 GetMoving();
Uint32 GetHP();
void SetHP(Uint32 newHP);
float GetShootRate();
void SetShootRate(float newShootRate);
float GetBulletSpeed();
void SetBulletSpeed(float newBulletSpeed);
float GetBulletRange();
void SetBulletRange(float newBulletRange);

#endif
