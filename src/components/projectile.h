#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "SDL_render.h"
#define MAX_PROJECTILES 100

#include "stdbool.h"

typedef struct {
  float x;
  float y;
  float vx;
  float vy;
  float distanceTraveled;
  bool active;
} Projectile;

void InitProjectiles();
void DrawProjectiles(SDL_Renderer *ren, float deltaTime, float currentTime);
void DestroyProjectiles();

#endif
