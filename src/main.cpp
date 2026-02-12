#include "raylib.h"
#include <cmath>
#include <ctime>
#include <raymath.h>

#include "Player.h"

float scale = 4;

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(800, 600, "PAAVO");
  MaximizeWindow();
  SetTargetFPS(GetMonitorRefreshRate(0));

  Player *player = new Player();

  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();

    player->update(deltaTime);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    player->draw();

    DrawFPS(0, 0);
    EndDrawing();
  }
  
  CloseWindow();
  return 0;
}
