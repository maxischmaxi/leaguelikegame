#include "projectile.h"
#include "../window.h"
#include "SDL_render.h"
#include "autoshoot-status-indicator.h"
#include "player.h"
#include <stdbool.h>
#include <stdlib.h>

static Projectile bullets[MAX_PROJECTILES];
static Uint32 lastShotTime = 0;

void InitProjectiles() {
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    bullets[i].active = false;
  }
}

void DrawProjectiles(SDL_Renderer *ren, float deltaTime, float currentTime) {
  bool autoShoot = GetAutoShoot();

  float timeSinceLastShot = (currentTime - lastShotTime) / 1000.0f;
  float shootRate = GetShootRate();
  float bulletSpeed = GetBulletSpeed();
  float bulletRange = GetBulletRange();
  Vector2 windowSize = GetWindowSize();

  if (autoShoot) {
    if (timeSinceLastShot >= (1.0f / shootRate)) {
      for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!bullets[i].active) {
          bullets[i].active = true;
          bullets[i].x = windowSize.x / 2;
          bullets[i].y = windowSize.y / 2;
          float angle = (float)(rand() % 360) * (M_PI / 180.0f);
          bullets[i].vx = cosf(angle) * bulletSpeed;
          bullets[i].vy = sinf(angle) * bulletSpeed;
          bullets[i].distanceTraveled = 0;
          lastShotTime = currentTime;
          break;
        }
      }
    }
  }

  for (int i = 0; i < MAX_PROJECTILES; i++) {
    if (bullets[i].active) {
      float dx = bullets[i].vx * deltaTime;
      float dy = bullets[i].vy * deltaTime;
      bullets[i].x += dx;
      bullets[i].y += dy;
      bullets[i].distanceTraveled += sqrtf(dx * dx + dy * dy);
      if (bullets[i].distanceTraveled > bulletRange) {
        bullets[i].active = false;
      }
    }
  }

  SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    if (bullets[i].active) {
      SDL_Rect bulletRect;
      bulletRect.x = (int)bullets[i].x;
      bulletRect.y = (int)bullets[i].y;
      bulletRect.w = 5;
      bulletRect.h = 5;
      SDL_RenderFillRect(ren, &bulletRect);
    }
  }
}

void DestroyProjectiles() {
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    bullets[i].active = false;
  }
}
