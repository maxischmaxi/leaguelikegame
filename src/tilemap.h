#ifndef TILEMAP_H
#define TILEMAP_H

#include "SDL_rect.h"
#include "SDL_render.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAP_WIDTH 25
#define MAP_HEIGHT 18
#define MAX_TILES_X 37
#define MAX_TILES_Y 23
#define TILE_SIZE 32

enum {
  TILE_EMPTY = 0,
  TILE_WALL_TOP = 5,
  TILE_WALL_LEFT = 75,
  TILE_WALL_TOP_LEFT = 2,
  TILE_WALL_TOP_RIGHT = 8,
  TILE_WALL_RIGHT = 83,
  TILE_WALL_BOTTOM_LEFT = 334,
  TILE_FLOOR = 71,
  TILE_WALL_BOTTOM = 374,
  TILE_WALL_BOTTOM_RIGHT = 378,
  TILE_DOOR,
  TILE_COLUMN,
};

int mapData[MAP_HEIGHT][MAP_WIDTH];
SDL_Rect GetTileRect(int tileIndex);
void DrawTile(SDL_Texture *tiles, SDL_Renderer *renderer, int tile, int x,
              int y);
int *IsTileBlocking(int tile);
SDL_Rect GetTileRectFromMap(int row, int col, float cameraX, float cameraY);

#endif
