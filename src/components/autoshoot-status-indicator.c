#include "../window.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"
#include <stdbool.h>

static bool autoShoot = false;
static SDL_Surface *autoShootSurface = NULL;
static SDL_Color color = {255, 255, 255, 255};
static SDL_Texture *textTexture = NULL;
static SDL_Rect textRect = {0, 0, 0, 0};

static char *GetAutoShootText() {
  return autoShoot ? "Auto Shoot: On" : "Auto Shoot: Off";
}

static void UpdateTextRect() {
  Vector2 windowSize = GetWindowSize();
  float scale = GetScale();

  textRect.w = autoShootSurface->w;
  textRect.h = autoShootSurface->h;
  textRect.x = 10;
  textRect.y = (windowSize.y / scale) - textRect.h - 20;
}

static void CreateAutoShootText(SDL_Renderer *ren, TTF_Font *font) {
  autoShootSurface = TTF_RenderText_Blended(font, GetAutoShootText(), color);
  textTexture = SDL_CreateTextureFromSurface(ren, autoShootSurface);
  SDL_FreeSurface(autoShootSurface);
  UpdateTextRect();
}

void InitAutoShootIndicator(SDL_Renderer *ren, TTF_Font *font) {
  CreateAutoShootText(ren, font);
  SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
}

void ToggleAutoShoot(SDL_Renderer *ren, TTF_Font *font) {
  autoShoot = autoShoot ? false : true;
  SDL_DestroyTexture(textTexture);
  CreateAutoShootText(ren, font);
  SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
}

void DrawAutoShootIndicator(SDL_Renderer *ren) {
  SDL_RenderCopy(ren, textTexture, NULL, &textRect);
}

void DestroyAutoShootIndicator() { SDL_DestroyTexture(textTexture); }

bool GetAutoShoot() { return autoShoot; }

void SetAutoShoot(bool value) { autoShoot = value; }
