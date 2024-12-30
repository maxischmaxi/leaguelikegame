#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_log.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include "tilemap.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>

// 33, 34, 35

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

static Uint32 lastTime = 0;
static Uint32 surfaceIndex = 0;
static float accumulator = 0.0f;
static float snapThreshhold = 3.0f;
static float cameraX = 0.0f;
static float cameraY = 0.0f;

float speed = 100.0f;
float playerX = 0.0f;
float playerY = 0.0f;
float targetX = 0.0f;
float targetY = 0.0f;
float scale = 3.0f;

int gameRunning = 0;
int playerMoving = 0;
int rightMouseDown = 0;
int keyDownW = 0;
int keyDownS = 0;
int keyDownA = 0;
int keyDownD = 0;

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    SDL_Log("IMG_Init Error: %s\n", IMG_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Window *win =
      SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (!win) {
    SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  if (!ren) {
    SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    return 1;
  }
  SDL_RenderSetScale(ren, scale, scale);

  SDL_Surface *tileSurface = IMG_Load("./assets/tech/tileset x1.png");
  if (!tileSurface) {
    SDL_Log("IMG_Load Error: %s\n", IMG_GetError());
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    return 1;
  }
  SDL_Texture *tiles = SDL_CreateTextureFromSurface(ren, tileSurface);
  SDL_FreeSurface(tileSurface);

  SDL_Surface *idleSurface = IMG_Load("./assets/the_adventurer/idle/idle.png");
  if (!idleSurface) {
    SDL_Log("IMG_Load Error: %s\n", IMG_GetError());
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    return 1;
  }
  SDL_Texture *idleTexture = SDL_CreateTextureFromSurface(ren, idleSurface);
  SDL_FreeSurface(idleSurface);

  SDL_Surface *walkSurface = IMG_Load("./assets/the_adventurer/walk/walk.png");
  if (!walkSurface) {
    SDL_Log("IMG_Load Error: %s\n", IMG_GetError());
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    return 1;
  }
  SDL_Texture *walkTexture = SDL_CreateTextureFromSurface(ren, walkSurface);
  SDL_FreeSurface(walkSurface);

  while (gameRunning) {
    if (lastTime == 0)
      lastTime = SDL_GetTicks();
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    accumulator += deltaTime;

    float accumulatorThreshold = 0.075f * (speed / 100.0f);

    if (accumulator >= accumulatorThreshold) {
      surfaceIndex = (surfaceIndex + 1) % 8;
      accumulator -= accumulatorThreshold;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        gameRunning = 0;
        break;
      }

      if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
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
      }

      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
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
      }

      if (event.type == SDL_MOUSEBUTTONDOWN &&
          event.button.button == SDL_BUTTON_RIGHT) {
        rightMouseDown = 1;
        targetX = (event.motion.x / scale) - 24.0f + cameraX;
        targetY = (event.motion.y / scale) - 32.0f + cameraY;
      }

      if (event.type == SDL_MOUSEMOTION && rightMouseDown) {
        targetX = (event.motion.x / scale) - 24.0f + cameraX;
        targetY = (event.motion.y / scale) - 32.0f + cameraY;
      }

      if (event.type == SDL_MOUSEBUTTONUP &&
          event.button.button == SDL_BUTTON_RIGHT) {
        rightMouseDown = 0;
        targetX = (event.motion.x / scale) - 24.0f + cameraX;
        targetY = (event.motion.y / scale) - 32.0f + cameraY;
      }
    }

    float dx = targetX - playerX;
    float dy = targetY - playerY;
    float dist = sqrt(dx * dx + dy * dy);

    int dirX = 0;
    int dirY = 0;

    if (fabsf(dx) > 0.01f) {
      dirX = (dx > 0) ? 1 : -1; // if dx is greater 0, right, else left
    }
    if (fabsf(dy) > 0.01f) {
      dirY = (dy > 0) ? 1 : -1; // if dy is greated 0, down, else up
    }

    if (dist < snapThreshhold) {
      playerX = targetX;
      playerY = targetY;
      playerMoving = 0;
    } else {
      float vx = (dx / dist) * speed * deltaTime;
      float vy = (dy / dist) * speed * deltaTime;
      playerX += vx;
      playerY += vy;
      playerMoving = 1;
    }

    cameraX = playerX - (SCREEN_WIDTH / (2.0f * scale));
    cameraY = playerY - (SCREEN_HEIGHT / (2.0f * scale));

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
    } else if (dirX == -1 && dirY == 0) {
      positionIndex = 1;
    } else if (dirX == 0 && dirY == 1) {
      positionIndex = 3;
    } else if (dirX == 0 && dirY == -1) {
      positionIndex = 0;
    }

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

    for (int row = 0; row < MAP_HEIGHT; row++) {
      for (int col = 0; col < MAP_WIDTH; col++) {
        int tile = mapData[row][col];
        int posX = col * TILE_SIZE;
        int posY = row * TILE_SIZE;
        DrawTile(tiles, ren, tile, posX - (int)cameraX, posY - (int)cameraY);
      }
    }

    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_Rect srcRect = {surfaceIndex * 48, positionIndex * 64, 48, 64};

    int playerPosX = (int)(playerX - cameraX);
    int playerPosY = (int)(playerY - cameraY);

    int hitboxX = playerPosX + 8;
    int hitboxY = playerPosY + 16;
    SDL_Rect hitbox = {hitboxX, hitboxY, 32, 32};

    for (int row = 0; row < MAP_HEIGHT; row++) {
      for (int col = 0; col < MAP_WIDTH; col++) {
        int tile = mapData[row][col];
        int posX = col * TILE_SIZE;
        int posY = row * TILE_SIZE;
        SDL_Rect tileRect = {posX - (int)cameraX, posY - (int)cameraY,
                             TILE_SIZE, TILE_SIZE};

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
    SDL_RenderCopy(ren, playerMoving ? walkTexture : idleTexture, &srcRect,
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

  return 0;
}
