#include "healthbar.h"
#include "../base.h"
#include "SDL_render.h"
#include "player.h"

static SDL_Texture *healthbarTexture = NULL;

void DrawHealthbar(SDL_Renderer *ren) {
  Uint32 hp = GetHP();
  SDL_Rect srcRect = {195 - (hp * 48), 21, 42, 7};
  SDL_Rect destRect = {5, 5, 42, 7};
  SDL_RenderCopy(ren, healthbarTexture, &srcRect, &destRect);
}

void InitHealthbar(SDL_Renderer *ren, SDL_Window *win) {
  healthbarTexture = LoadHealthbarTexture(ren, win);
}

void DestroyHealthbar() {
  if (healthbarTexture) {
    SDL_DestroyTexture(healthbarTexture);
  }
}
