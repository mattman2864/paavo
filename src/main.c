#include "raylib.h"

int main(void) {
  SetConfigFlags(FLAG_FULLSCREEN_MODE);  
  InitWindow(800, 600, "Hello Raylib");
  SetTargetFPS(60);

  Texture2D ship = LoadTexture("assets/ship.png");
  Vector2 pos = {0, 0};
  float speed = 300.f;

  Color bgcolor = { 0 };
  bgcolor.a = 0xff;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    if (IsKeyDown(KEY_E)) pos.y -= speed * dt;
    if (IsKeyDown(KEY_S)) pos.x -= speed * dt;
    if (IsKeyDown(KEY_D)) pos.y += speed * dt;
    if (IsKeyDown(KEY_F)) pos.x += speed * dt;

    BeginDrawing();

      ClearBackground(bgcolor);
      DrawTextureEx(ship, pos, 0, 4, WHITE);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
