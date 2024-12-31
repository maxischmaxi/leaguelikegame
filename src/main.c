#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include "base.h"
#include "player.h"
#include "tilemap.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>

Uint32 lastTime = 0;
Uint32 surfaceIndex = 0;
float accumulator = 0.0f;
float cameraX = 0.0f;
float cameraY = 0.0f;
float speed = 100.0f;
float playerX = 0.0f;
float playerY = 0.0f;
float scale = 3.0f;
int gameRunning = 1;
int moving = 0;
int keyDownW = 0;
int keyDownS = 0;
int keyDownA = 0;
int keyDownD = 0;
int autoShoot = 0;

void HandleKeyUp(SDL_Event *event) {
  if (event->type == SDL_KEYUP) {
    switch (event->key.keysym.sym) {
    case SDLK_w:
      keyDownW = 0;
      break;
    case SDLK_s:
      keyDownS = 0;
      break;
    case SDLK_a:
      keyDownA = 0;
      break;
    case SDLK_d:
      keyDownD = 0;
      break;
    default:
      break;
    }

    if (!keyDownW && !keyDownS && !keyDownA && !keyDownD) {
      moving = 0;
    }
  }
}

void HandleKeyDown(SDL_Event *event) {
  if (event->type == SDL_KEYDOWN) {
    switch (event->key.keysym.sym) {
    case SDLK_SPACE:
      autoShoot = autoShoot ? 0 : 1;
      break;
    case SDLK_w:
      keyDownW = 1;
      break;
    case SDLK_s:
      keyDownS = 1;
      break;
    case SDLK_a:
      keyDownA = 1;
      break;
    case SDLK_d:
      keyDownD = 1;
      break;
    default:
      break;
    }

    moving = keyDownW || keyDownS || keyDownA || keyDownD;
  }
}

int main(int argc, char *argv[]) {
  SDL_Window *win = Init();
  SDL_Renderer *ren = InitRenderer(win, &scale);
  SDL_Texture *tiles = LoadTileTexture(ren, win);
  SDL_Texture *idleTexture = LoadIdleTexture(ren, win);
  SDL_Texture *walkTexture = LoadWalkTexture(ren, win);

  while (gameRunning) {
    if (lastTime == 0)
      lastTime = SDL_GetTicks();
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    accumulator += deltaTime;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        gameRunning = 0;
        break;
      }

      HandleKeyUp(&event);
      HandleKeyDown(&event);
    }

    float appliedSpeed = speed / 100.0f;
    CalculateAccumulator(&moving, &appliedSpeed, &accumulator, &surfaceIndex);

    int dirX = keyDownA ? -1 : keyDownD ? 1 : 0;
    int dirY = keyDownW ? -1 : keyDownS ? 1 : 0;

    playerX += dirX * appliedSpeed;
    playerY += dirY * appliedSpeed;
    cameraX = playerX - (SCREEN_WIDTH / (2.0f * scale));
    cameraY = playerY - (SCREEN_HEIGHT / (2.0f * scale));

    DrawBlackScreen(ren);

    for (int row = 0; row < MAP_HEIGHT; row++) {
      for (int col = 0; col < MAP_WIDTH; col++) {
        int tile = mapData[row][col];
        int posX = (col * TILE_SIZE) - (int)cameraX;
        int posY = (row * TILE_SIZE) - (int)cameraY;
        DrawTile(tiles, ren, tile, posX, posY);
      }
    }

    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    unsigned int spriteIndex = GetSpriteIndex(dirX, dirY);
    SDL_Rect srcRect = {surfaceIndex * 48, spriteIndex * 64, 48, 64};

    int playerPosX = (int)(playerX - cameraX);
    int playerPosY = (int)(playerY - cameraY);

    SDL_Rect hitbox = GetPlayerHitbox(playerPosX, playerPosY);
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

    SDL_Rect destRect = {playerPosX, playerPosY, 48, 64};
    SDL_RenderCopy(ren, moving ? walkTexture : idleTexture, &srcRect,
                   &destRect);
    SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
    SDL_RenderDrawRect(ren, &hitbox);
    SDL_RenderPresent(ren);
    SDL_Delay(16);
  }

  SDL_DestroyTexture(tiles);
  SDL_DestroyTexture(idleTexture);
  SDL_DestroyTexture(walkTexture);

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  IMG_Quit();
  SDL_Quit();

  printf("Game exited successfully\n");
  return 0;
}
