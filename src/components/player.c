#include "../base.h"
#include "../tilemap.h"
#include "../vector.h"
#include "../window.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"

static float bulletRange = 400.0f;
static float bulletSpeed = 200.0f;
static float shootRate = 1.0f;
static Uint32 hp = 2;
static float accumulator = 0.0f;
static Uint32 spriteIndex = 0;
static Uint32 surfaceIndex = 0;
static int dirX = 0;
static int dirY = 0;
static int lastDirX = 0;
static int lastDirY = 0;
static float playerX = 0.0f;
static float playerY = 0.0f;
static SDL_Texture *playerTexture = NULL;
static Uint8 moving = 0;
static SDL_Texture *walkTexture = NULL;
static SDL_Texture *idleTexture = NULL;
static float speed = 100.0f;

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

static SDL_Rect GetTileRectFromMap(int row, int col, float cameraX,
                                   float cameraY) {
  int posX = col * TILE_SIZE;
  int posY = row * TILE_SIZE;
  int tileX = posX - (int)cameraX;
  int tileY = posY - (int)cameraY;

  return (SDL_Rect){tileX, tileY, TILE_SIZE, TILE_SIZE};
}

SDL_Rect GetPlayerHitbox() {
  Vector2 windowSize = GetWindowSize();
  float scale = GetScale();
  float cameraX =
      playerX - (windowSize.x / (2.0f * scale)) + 24; // +24 to center
  float cameraY =
      playerY - (windowSize.y / (2.0f * scale)) + 32; // +32 to center
  int playerPosX = (int)(playerX - cameraX);
  int playerPosY = (int)(playerY - cameraY);
  int hitboxX = playerPosX + 8;
  int hitboxY = playerPosY + 16;
  return (SDL_Rect){hitboxX, hitboxY, TILE_SIZE, TILE_SIZE};
}

Vector2 GetPlayerPosition() { return (Vector2){playerX, playerY}; }

void InitPlayer(SDL_Renderer *ren, SDL_Window *win) {
  walkTexture = LoadWalkTexture(ren, win);
  idleTexture = LoadIdleTexture(ren, win);
}

void SetMoving(int value) { moving = value; }

Uint8 GetMoving() { return moving; }

void DrawPlayer(SDL_Renderer *ren, float deltaTime) {
  float appliedSpeed = speed / 100.0f;
  float accumulatorThreshold = (moving ? 0.075f : 0.25f) * appliedSpeed;
  accumulator += deltaTime;

  if (accumulator >= accumulatorThreshold) {
    surfaceIndex = (surfaceIndex + 1) % 8;
    accumulator -= accumulatorThreshold;

    if (hp == 0) {
      hp = 4;
    } else {
      hp--;
    }
  }

  if (dirX != 0) {
    lastDirX = dirX;

    if (dirY == 0) {
      lastDirY = 0;
    }
  }
  if (dirY != 0) {
    lastDirY = dirY;

    if (dirX == 0) {
      lastDirX = 0;
    }
  }

  Vector2 windowSize = GetWindowSize();
  float scale = GetScale();
  float cameraX =
      playerX - (windowSize.x / (2.0f * scale)) + 24; // +24 to center
  float cameraY =
      playerY - (windowSize.y / (2.0f * scale)) + 32; // +32 to center
  int playerPosX = (int)(playerX - cameraX);
  int playerPosY = (int)(playerY - cameraY);
  KeyDown keyDown = GetKeyDown();
  dirX = keyDown.a ? -1 : keyDown.d ? 1 : 0;
  dirY = keyDown.w ? -1 : keyDown.s ? 1 : 0;
  playerX += dirX * appliedSpeed;
  playerY += dirY * appliedSpeed;

  SDL_Rect hitbox = GetPlayerHitbox();
  for (int row = 0; row < MAP_HEIGHT; row++) {
    for (int col = 0; col < MAP_WIDTH; col++) {
      SDL_Rect tileRect = GetTileRectFromMap(row, col, cameraX, cameraY);
      int tile = mapData[row][col];

      if (SDL_HasIntersection(&hitbox, &tileRect)) {
        int *blocking = IsTileBlocking(tile);
        if (blocking) {
          SDL_Rect result;
          SDL_IntersectRect(&hitbox, &tileRect, &result);

          if (result.w > 24 || result.h > 20) {
            SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
            SDL_RenderDrawRect(ren, &tileRect);
          }
        }
      }
    }
  }

  spriteIndex = GetSpriteIndex(dirX, dirY, lastDirX, lastDirY);
  SDL_Rect srcRect = {surfaceIndex * 48, spriteIndex * 64, 48, 64};
  SDL_Rect destRect = {playerPosX, playerPosY, 48, 64};
  playerTexture = moving ? walkTexture : idleTexture;
  SDL_RenderCopy(ren, playerTexture, &srcRect, &destRect);
  SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
  SDL_RenderDrawRect(ren, &hitbox);
}

void DestroyPlayer() {
  SDL_DestroyTexture(playerTexture);
  SDL_DestroyTexture(idleTexture);
  SDL_DestroyTexture(walkTexture);
}

void SetHP(Uint32 newHP) { hp = newHP; }

Uint32 GetHP() { return hp; }

float GetSpeed() { return speed; }

float GetShootRate() { return shootRate; }

void SetShootRate(float newShootRate) { shootRate = newShootRate; }

float GetBulletSpeed() { return bulletSpeed; }

void SetBulletSpeed(float newBulletSpeed) { bulletSpeed = newBulletSpeed; }

float GetBulletRange() { return bulletRange; }

void SetBulletRange(float newBulletRange) { bulletRange = newBulletRange; }
