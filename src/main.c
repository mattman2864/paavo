#include "raylib.h"
#include "ship.h"
#include <raymath.h>

void draw_grid(Camera2D cam, int spacing, Color color) {
    Vector2 topLeft = GetScreenToWorld2D((Vector2){0, 0}, cam);
    Vector2 botRight = GetScreenToWorld2D((Vector2){GetScreenWidth(), GetScreenHeight()}, cam);

    int startX = (int)(topLeft.x / spacing) * spacing;
    int startY = (int)(topLeft.y / spacing) * spacing;

    for (int x = startX; x < botRight.x; x += spacing)
        DrawLine(x, topLeft.y, x, botRight.y, color);
    for (int y = startY; y < botRight.y; y += spacing)
        DrawLine(topLeft.x, y, botRight.x, y, color);
}

int main(void) {
  SetConfigFlags(FLAG_FULLSCREEN_MODE);
  InitWindow(800, 600, "PAAVO");
  SetTargetFPS(144);

  ship_t* ship = create_ship((Vector2){ 0 }, 800, 10);

  Color bgcolor = { 0 };
  bgcolor.a = 0xff;

  Camera2D cam;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    if (IsKeyDown(KEY_SPACE)) {
      ship->acc = 4000;
    } else {
      ship->acc = 800;
    }

    Vector2 dir = {
        IsKeyDown(KEY_E) - IsKeyDown(KEY_D),
        IsKeyDown(KEY_F) - IsKeyDown(KEY_S)};
    move_ship(ship, (Vector2){GetMouseX(), GetMouseY()},
        dir, dt);

    cam = (Camera2D){
      .offset = (Vector2){GetScreenWidth()/2., GetScreenHeight()/2.},
      .target = ship->pos,
      .zoom = 1,
    };

    BeginDrawing();
      BeginMode2D(cam);
        ClearBackground(bgcolor);
        draw_grid(cam, 200, GRAY);
        DrawCircle(0, 0, 50, RED);
        DrawCircle(200, 0, 50, BLUE);
        draw_ship(ship);
      EndMode2D();
      DrawFPS(0, 0);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
