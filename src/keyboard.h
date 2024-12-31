#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "SDL_events.h"
void HandleKeyUp(SDL_Event *event, int *keyDownW, int *keyDownS, int *keyDownA,
                 int *keyDownD, int *playerMoving);
void HandleKeyDown(SDL_Event *event, int *keyDownW, int *keyDownS,
                   int *keyDownA, int *keyDownD, int *playerMoving);

#endif
