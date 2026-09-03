#include "raylib.h"
#include "entity.h"
#include "stdlib.h"
#include <raymath.h>

int main(void) {
  SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE);
  InitWindow(800, 600, "PAAVO");
  SetTargetFPS(GetMonitorRefreshRate(0));

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();


    BeginDrawing();
    ClearBackground(BLACK);
    DrawFPS(0, 0);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
