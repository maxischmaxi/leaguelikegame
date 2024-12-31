#include "SDL_events.h"
#include "SDL_keycode.h"

void HandleKeyUp(SDL_Event *event, int *keyDownW, int *keyDownS, int *keyDownA,
                 int *keyDownD, int *playerMoving) {
  if (event->type == SDL_KEYUP) {
    switch (event->key.keysym.sym) {
    case SDLK_w:
      *keyDownW = 0;
      break;
    case SDLK_s:
      *keyDownS = 0;
      break;
    case SDLK_a:
      *keyDownA = 0;
      break;
    case SDLK_d:
      *keyDownD = 0;
      break;
    default:
      break;
    }

    if (!keyDownW && !keyDownS && !keyDownA && !keyDownD) {
      *playerMoving = 0;
    }
  }
}

void HandleKeyDown(SDL_Event *event, int *keyDownW, int *keyDownS,
                   int *keyDownA, int *keyDownD, int *playerMoving) {
  if (event->type == SDL_KEYDOWN) {
    switch (event->key.keysym.sym) {
    case SDLK_w:
      *keyDownW = 1;
      break;
    case SDLK_s:
      *keyDownS = 1;
      break;
    case SDLK_a:
      *keyDownA = 1;
      break;
    case SDLK_d:
      *keyDownD = 1;
      break;
    default:
      break;
    }

    *playerMoving = keyDownW || keyDownS || keyDownA || keyDownD;
  }
}
