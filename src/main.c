#include "raylib.h"
#include "penguin.h"
#include "stdlib.h"
#include <raymath.h>

int main(void) {
  SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE);
  InitWindow(800, 600, "PAAVO");
  Penguin *p = malloc(sizeof(Penguin));
  p->x = 0;
  p->y = 0;
  p->width = 64;
  p->height = 128;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    Vector2 move = {
        IsKeyDown(KEY_F) - IsKeyDown(KEY_S),
        IsKeyDown(KEY_D) - IsKeyDown(KEY_E)
        };
    movePenguin(p, move, dt);



    BeginDrawing();
    ClearBackground(BLACK);
    drawPenguin(p);
    DrawFPS(0, 0);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
