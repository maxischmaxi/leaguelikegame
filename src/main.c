#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include "SDL_video.h"
#include "base.h"
#include "components/autoshoot-status-indicator.h"
#include "components/healthbar.h"
#include "components/player.h"
#include "components/projectile.h"
#include "tilemap.h"
#include "window.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>

Uint32 lastTime = 0;
int gameRunning = 1;

int main(int argc, char *argv[]) {
  SDL_Window *win = InitWindow();
  SDL_Renderer *ren = InitRenderer(win);
  TTF_Font *font = TTF_OpenFont(
      "./assets/Pixelify_Sans/static/PixelifySans-Regular.ttf", 11);

  InitHealthbar(ren, win);
  InitTilemap(ren, win);
  InitPlayer(ren, win);
  InitAutoShootIndicator(ren, font);
  InitProjectiles();

  while (gameRunning) {
    if (lastTime == 0) {
      lastTime = SDL_GetTicks();
    }

    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        gameRunning = 0;
      }

      if (event.type == SDL_WINDOWEVENT) {
        OnWindowEvent(ren, win, event);
      }

      if (event.type == SDL_KEYUP) {
        OnKeyUpEvent(event);
      }

      if (event.type == SDL_KEYDOWN) {
        OnKeyDownEvent(event, ren, font);
      }
    }

    OnRender();
    DrawBlackScreen(ren);

    DrawTilemap(ren);
    DrawPlayer(ren, deltaTime);
    DrawProjectiles(ren, deltaTime, currentTime);
    DrawHealthbar(ren);
    DrawAutoShootIndicator(ren);

    SDL_RenderPresent(ren);
    SDL_Delay(16);
  }

  DestroyHealthbar();
  DestroyTilemap();
  DestroyPlayer();
  DestroyAutoShootIndicator();
  DestroyProjectiles();

  TTF_CloseFont(font);

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  printf("Game exited successfully\n");
  return 0;
}
