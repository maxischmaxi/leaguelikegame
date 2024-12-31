#include "SDL_rect.h"
#include "tilemap.h"

Uint32 GetSpriteIndex(int dirX, int dirY, int lastDirX, int lastDirY) {
  Uint32 spriteIndex = 0;

  if (dirX == 1 && dirY == 1) { // right down
    spriteIndex = 5;
  } else if (dirX == 1 && dirY == -1) { // right up
    spriteIndex = 4;
  } else if (dirX == -1 && dirY == 1) { // left down
    spriteIndex = 1;
  } else if (dirX == -1 && dirY == -1) { // left up
    spriteIndex = 2;
  } else if (dirX == 1 && dirY == 0) { // right
    spriteIndex = 5;
  } else if (dirX == -1 && dirY == 0) { // left
    spriteIndex = 1;
  } else if (dirX == 0 && dirY == 1) { // down
    spriteIndex = 0;
  } else if (dirX == 0 && dirY == -1) { // up
    spriteIndex = 3;
  } else {
    if (lastDirX == 1 && lastDirY == -1) { // right up
      spriteIndex = 4;
    } else if (lastDirX == -1 && lastDirY == 1) { // left down
      spriteIndex = 1;
    } else if (lastDirX == -1 && lastDirY == -1) { // left up
      spriteIndex = 2;
    } else if (lastDirX == 1 && lastDirY == 0) { // right
      spriteIndex = 5;
    } else if (lastDirX == -1 && lastDirY == 0) { // left
      spriteIndex = 1;
    } else if (lastDirX == 0 && lastDirY == 1) { // down
      spriteIndex = 0;
    } else if (lastDirX == 0 && lastDirY == -1) { // up
      spriteIndex = 3;
    } 
  }

  return spriteIndex;
}

SDL_Rect GetPlayerHitbox(int playerPosX, int playerPosY) {
  int hitboxX = playerPosX + 8;
  int hitboxY = playerPosY + 16;
  return (SDL_Rect){hitboxX, hitboxY, TILE_SIZE, TILE_SIZE};
}
