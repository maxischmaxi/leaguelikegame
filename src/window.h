#ifndef WINDOW_H
#define WINDOW_H

#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_ttf.h"
#include "SDL_video.h"
#include "vector.h"

#define CONFIG_FILE "./config"

typedef struct {
  int w, a, s, d;
} KeyDown;

SDL_Window *InitWindow();
Vector2 GetWindowSize();
Uint8 GetFullscreen();
void SetFullscreen(Uint8 value, SDL_Window *win);
void SetWindowSize(Vector2 size);
void OnWindowEvent(SDL_Renderer *ren, SDL_Window *win, SDL_Event event);
void OnKeyUpEvent(SDL_Event event);
void OnKeyDownEvent(SDL_Event event, SDL_Renderer *ren, TTF_Font *font);
KeyDown GetKeyDown();
Vector2 GetCameraPosition();
void OnRender();
float GetScale();

#endif
