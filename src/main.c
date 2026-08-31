#include "raylib.h"
#include "entity.h"
#include "stdlib.h"
#include <raymath.h>

int main(void) {
  SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE);
  InitWindow(800, 600, "PAAVO");


  Animation ani = {.frame = 0, .framerate = 4, .num_frames = 4, .source = LoadTextureFromImage(LoadImage("assets/paavo_spritesheet.png")),
                   .size = (Vector2){.x = 32, .y = 32}, .scale = 8.0};
  Entity *e = malloc(sizeof(Entity));
  e->pos = (Vector2){0, 0};
  e->animation = &ani;
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    Vector2 move = {
        IsKeyDown(KEY_F) - IsKeyDown(KEY_S),
        IsKeyDown(KEY_D) - IsKeyDown(KEY_E)
        };
    move_entity(e, move, dt);
    update_entity(e);



    BeginDrawing();
    ClearBackground(BLACK);
    draw_entity(e);
    DrawFPS(0, 0);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
