#include "SDL_image.h"
#include "SDL_log.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include "tilemap.h"
#include <stdlib.h>

void DrawBlackScreen(SDL_Renderer *ren) {
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  SDL_RenderClear(ren);
}

void CalculateAccumulator(int *moving, float *appliedSpeed, float *accumulator,
                          unsigned int *surfaceIndex) {
  float accumulatorThreshold = (*moving ? 0.075f : 0.25f) * *appliedSpeed;

  if (*accumulator >= accumulatorThreshold) {
    *surfaceIndex = (*surfaceIndex + 1) % 8;
    *accumulator -= accumulatorThreshold;
  }
}

SDL_Window *Init() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
    exit(1);
  }

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    SDL_Log("IMG_Init Error: %s\n", IMG_GetError());
    SDL_Quit();
    exit(1);
  }

  SDL_Window *win =
      SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (!win) {
    SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
    IMG_Quit();
    SDL_Quit();
    exit(1);
  }

  return win;
}

SDL_Renderer *InitRenderer(SDL_Window *win, float *scale) {
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  if (!ren) {
    SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    exit(1);
  }

  SDL_RenderSetScale(ren, *scale, *scale);

  return ren;
}

SDL_Texture *LoadTileTexture(SDL_Renderer *ren, SDL_Window *win) {
  SDL_Surface *tileSurface = IMG_Load("./assets/tech/tileset x1.png");
  if (!tileSurface) {
    SDL_Log("IMG_Load Error: %s\n", IMG_GetError());
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    exit(1);
  }
  SDL_Texture *tiles = SDL_CreateTextureFromSurface(ren, tileSurface);
  SDL_FreeSurface(tileSurface);

  return tiles;
}

SDL_Texture *LoadIdleTexture(SDL_Renderer *ren, SDL_Window *win) {
  SDL_Surface *idleSurface = IMG_Load("./assets/the_adventurer/idle/idle.png");
  if (!idleSurface) {
    SDL_Log("IMG_Load Error: %s\n", IMG_GetError());
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    exit(1);
  }

  SDL_Texture *idleTexture = SDL_CreateTextureFromSurface(ren, idleSurface);
  SDL_FreeSurface(idleSurface);
  return idleTexture;
}

SDL_Texture *LoadWalkTexture(SDL_Renderer *ren, SDL_Window *win) {
  SDL_Surface *walkSurface = IMG_Load("./assets/the_adventurer/walk/walk.png");
  if (!walkSurface) {
    SDL_Log("IMG_Load Error: %s\n", IMG_GetError());
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    exit(1);
  }
  SDL_Texture *walkTexture = SDL_CreateTextureFromSurface(ren, walkSurface);
  SDL_FreeSurface(walkSurface);

  return walkTexture;
}
