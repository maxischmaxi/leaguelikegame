#include "window.h"
#include "SDL.h"
#include "SDL_events.h"
#include "SDL_image.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_ttf.h"
#include "SDL_video.h"
#include "components/autoshoot-status-indicator.h"
#include "components/player.h"
#include "vector.h"

static float scale = 3.0f;
static int keyDownW = 0;
static int keyDownS = 0;
static int keyDownA = 0;
static int keyDownD = 0;
static Vector2 windowSize = {1200, 800};
static Uint8 fullscreen = 0;

Vector2 cameraPosition = {0, 0};

static int configExists() {
  FILE *file;
  if ((file = fopen(CONFIG_FILE, "r"))) {
    fclose(file);
    return 1;
  }

  return 0;
}

static void createConfigFile() {
  FILE *file = fopen(CONFIG_FILE, "w");
  if (file == NULL) {
    return;
  }

  fprintf(file, "width=%d\nheight=%d\nfullscreen=%d\n", (int)windowSize.x,
          (int)windowSize.y, fullscreen);
  fclose(file);
}

static void LoadWindowSettingsFromFile() {
  if (configExists()) {
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    file = fopen(CONFIG_FILE, "r");
    if (file == NULL) {
      return;
    }

    while ((read = getline(&line, &len, file)) != -1) {
      if (strncmp(line, "width", 5) == 0) {
        windowSize.x = atof(line + 6);
      } else if (strncmp(line, "height", 6) == 0) {
        windowSize.y = atof(line + 7);
      } else if (strncmp(line, "fullscreen", 10) == 0) {
        fullscreen = atoi(line + 11);
      }
    }

    fclose(file);
    if (line) {
      free(line);
    }

    if (windowSize.x <= 0 || windowSize.y <= 0) {
      windowSize.x = 1200;
      windowSize.y = 800;
      fullscreen = 0;
    }
  } else {
    createConfigFile();
  }
}

static int SaveWindowSettingsToFile() {
  if (configExists()) {
    FILE *file = fopen(CONFIG_FILE, "w");
    if (file == NULL) {
      return -1;
    }

    fprintf(file, "width=%d\nheight=%d\nfullscreen=%d\n", (int)windowSize.x,
            (int)windowSize.y, fullscreen);
    fclose(file);
    return 0;
  }

  createConfigFile();
  return 0;
}

float GetScale() { return scale; }

void OnRender() {
  Vector2 windowSize = GetWindowSize();
  Vector2 playerPos = GetPlayerPosition();
  float scale = GetScale();
  cameraPosition.x =
      playerPos.x - (windowSize.x / (2.0f * scale)) + 24; // +24 to center
  cameraPosition.y =
      playerPos.y - (windowSize.y / (2.0f * scale)) + 32; // +32 to center
}

KeyDown GetKeyDown() {
  return (KeyDown){keyDownW, keyDownA, keyDownS, keyDownD};
}

void SetWindowSize(Vector2 size) {
  windowSize.x = size.x;
  windowSize.y = size.y;
  SaveWindowSettingsToFile();
}

void OnKeyUpEvent(SDL_Event event) {
  switch (event.key.keysym.sym) {
  case SDLK_w:
    keyDownW = 0;
    break;
  case SDLK_s:
    keyDownS = 0;
    break;
  case SDLK_a:
    keyDownA = 0;
    break;
  case SDLK_d:
    keyDownD = 0;
    break;
  default:
    break;
  }

  SetMoving((!keyDownW && !keyDownS && !keyDownA && !keyDownD) ? 0 : 1);
}

void OnKeyDownEvent(SDL_Event event, SDL_Renderer *ren, TTF_Font *font) {
  switch (event.key.keysym.sym) {
  case SDLK_SPACE: {
    ToggleAutoShoot(ren, font);
    break;
  }
  case SDLK_w:
    keyDownW = 1;
    break;
  case SDLK_s:
    keyDownS = 1;
    break;
  case SDLK_a:
    keyDownA = 1;
    break;
  case SDLK_d:
    keyDownD = 1;
    break;
  default:
    break;
  }

  SetMoving((keyDownW || keyDownS || keyDownA || keyDownD) ? 1 : 0);
}

Vector2 GetCameraPosition() { return cameraPosition; }

void OnWindowEvent(SDL_Renderer *ren, SDL_Window *win, SDL_Event event) {
  switch (event.window.event) {
  case SDL_WINDOWEVENT_RESIZED: {

    windowSize.x = event.window.data1;
    windowSize.y = event.window.data2;

    SaveWindowSettingsToFile();
    SDL_RenderSetViewport(ren, NULL);
    break;
  }
  default:
    break;
  }
}

Vector2 GetWindowSize() { return windowSize; }

Uint8 GetFullscreen() { return fullscreen; }

void SetFullscreen(Uint8 value, SDL_Window *win) {
  if (value) {
    SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
  } else {
    SDL_SetWindowFullscreen(win, 0);
  }

  fullscreen = value;
  SaveWindowSettingsToFile();
}

SDL_Window *InitWindow() {
  LoadWindowSettingsFromFile();

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
    exit(1);
  }

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    SDL_Log("IMG_Init Error: %s\n", IMG_GetError());
    SDL_Quit();
    exit(1);
  }

  TTF_Init();

  Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
  if (fullscreen) {
    flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
  }

  SDL_Window *win =
      SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       windowSize.x, windowSize.y, flags);

  if (!win) {
    SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    exit(1);
  }

  return win;
}
