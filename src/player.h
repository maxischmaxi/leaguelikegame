#ifndef PLAYER_H
#define PLAYER_H

#include "SDL_rect.h"

Uint32 GetSpriteIndex(int dirX, int dirY, int lastDirX, int lastDirY);
SDL_Rect GetPlayerHitbox(int playerPosX, int playerPosY);

#endif
