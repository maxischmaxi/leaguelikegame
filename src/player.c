#include "SDL_rect.h"
#include "tilemap.h"

unsigned int GetSpriteIndex(int dirX, int dirY) {
  // there are six different rows of sprites
  unsigned int positionIndex = 0;

  if (dirX == 1 && dirY == 1) { // right down
    positionIndex = 5;
  } else if (dirX == 1 && dirY == -1) { // right up
    positionIndex = 4;
  } else if (dirX == -1 && dirY == 1) { // left down
    positionIndex = 1;
  } else if (dirX == -1 && dirY == -1) { // left up
    positionIndex = 2;
  } else if (dirX == 1 && dirY == 0) { // right
    positionIndex = 5;
  } else if (dirX == -1 && dirY == 0) { // left
    positionIndex = 1;
  } else if (dirX == 0 && dirY == 1) { // down
    positionIndex = 0;
  } else if (dirX == 0 && dirY == -1) { // up
    positionIndex = 3;
  }

  return positionIndex;
}

SDL_Rect GetPlayerHitbox(int playerPosX, int playerPosY) {
  int hitboxX = playerPosX + 8;
  int hitboxY = playerPosY + 16;
  return (SDL_Rect){hitboxX, hitboxY, TILE_SIZE, TILE_SIZE};
}
