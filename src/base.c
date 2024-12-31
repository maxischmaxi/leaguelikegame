#include "SDL_image.h"
#include "SDL_log.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include "window.h"
#include <stdlib.h>

void DrawBlackScreen(SDL_Renderer *ren) {
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  SDL_RenderClear(ren);
}

SDL_Renderer *InitRenderer(SDL_Window *win) {
  float scale = GetScale();
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  if (!ren) {
    SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    exit(1);
  }

  SDL_RenderSetScale(ren, scale, scale);

  return ren;
}

SDL_Texture *LoadHealthbarTexture(SDL_Renderer *ren, SDL_Window *win) {
  SDL_Surface *healthbarSurface = IMG_Load("./assets/PixelUI/06.png");
  if (!healthbarSurface) {
    SDL_Log("IMG_Load Error: %s\n", IMG_GetError());
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    exit(1);
  }
  SDL_Texture *healthbarTexture =
      SDL_CreateTextureFromSurface(ren, healthbarSurface);
  SDL_FreeSurface(healthbarSurface);

  return healthbarTexture;
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
