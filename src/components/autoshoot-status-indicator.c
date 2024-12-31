#include "../tilemap.h"
#include "SDL_log.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"

static Uint32 autoShoot = 0;
static SDL_Surface *autoShootSurface = NULL;
static SDL_Color color = {255, 255, 255, 255};
static SDL_Texture *textTexture = NULL;
static SDL_Rect textRect = {0, 0, 0, 0};

static char *GetAutoShootText() {
  return autoShoot ? "Auto Shoot: On" : "Auto Shoot: Off";
}

static void UpdateTextRect(float *scale) {
  textRect.w = autoShootSurface->w;
  textRect.h = autoShootSurface->h;
  textRect.x = 10;
  textRect.y = (SCREEN_HEIGHT / *scale) - textRect.h - 20;
}

static void CreateAutoShootText(SDL_Renderer *ren, TTF_Font *font,
                                float *scale) {
  autoShootSurface = TTF_RenderText_Blended(font, GetAutoShootText(), color);
  textTexture = SDL_CreateTextureFromSurface(ren, autoShootSurface);
  SDL_FreeSurface(autoShootSurface);
  UpdateTextRect(scale);
}

void InitAutoShootIndicator(SDL_Renderer *ren, TTF_Font *font, float *scale) {
  CreateAutoShootText(ren, font, scale);
  SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
}

void ToggleAutoShoot(SDL_Renderer *ren, TTF_Font *font, float *scale) {
  autoShoot = autoShoot ? 0 : 1;
  SDL_DestroyTexture(textTexture);
  CreateAutoShootText(ren, font, scale);
  SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
}

void DrawAutoShootIndicator(SDL_Renderer *ren) {
  SDL_Log("x %d y %d w %d h %d", textRect.x, textRect.y, textRect.w,
          textRect.h);
  SDL_RenderCopy(ren, textTexture, NULL, &textRect);
}

void DestroyAutoShootIndicator() { SDL_DestroyTexture(textTexture); }
