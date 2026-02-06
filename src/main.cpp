#include "raylib.h"
#include <raymath.h>


int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(800, 600, "PAAVO");
  MaximizeWindow();
  SetTargetFPS(GetMonitorRefreshRate(0));

  Texture2D player_texture = LoadTexture("assets/entities/player/player.png");
  Vector2 pos = {0, 0};
  float speed = 300.f;

  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();

    Vector2 dir = {
      (float) IsKeyDown(KEY_F) - IsKeyDown(KEY_S),
      (float) IsKeyDown(KEY_D) - IsKeyDown(KEY_E)
    };
    dir = Vector2Normalize(dir);
    pos = Vector2Add(pos, Vector2Scale(dir, speed * deltaTime));

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawTextureEx(player_texture, pos, 0, 4.f, RAYWHITE);

    DrawFPS(0, 0);
    EndDrawing();
  }
  
  UnloadTexture(player_texture);
  CloseWindow();
  return 0;
}
