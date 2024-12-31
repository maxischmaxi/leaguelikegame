#include "SDL_render.h"
#include "base.h"

static SDL_Texture *healthbarTexture = NULL;
static Uint32 hp = 2;

void SetHP(Uint32 newHP) { hp = newHP; }

Uint32 GetHP() { return hp; }

void DrawHealthbar(SDL_Renderer *ren) {
  SDL_Rect srcRect = {195 - (hp * 48), 21, 42, 7};
  SDL_Rect destRect = {0, 10, 42, 7};
  SDL_RenderCopy(ren, healthbarTexture, &srcRect, &destRect);
}

void InitHealthbar(SDL_Renderer *ren, SDL_Window *win) {
  healthbarTexture = LoadHealthbarTexture(ren, win);
}
