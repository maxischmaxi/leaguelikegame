#ifndef PLAYER_H
#define PLAYER_H

#include "SDL_rect.h"
unsigned int GetSpriteIndex(int dirX, int dirY);
SDL_Rect GetPlayerHitbox(int playerPosX, int playerPosY);

#endif
