#include "raylib.h"
#include <raymath.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <math.h>

#define WALK_SPEED 300

typedef enum { MODE_SHIP, MODE_FOOT } Mode;
typedef struct { Vector2 pos, vel; float rot; } Ship;
typedef struct { Vector2 pos, vel; } Paavo;
typedef struct { Mode mode; Ship ship; Paavo paavo; Camera2D cam; } Game;

void paavo_draw_debug(Paavo* paavo) {
  DrawText(TextFormat("x: %.2f\ny: %.2f", paavo->pos.x, paavo->pos.y), 0, 20, 16, RAYWHITE);
}

static uint32_t hash2(int x, int y) {
  uint32_t h = (uint32_t)x * 374761393u + (uint32_t)y * 668265263u;
  h = (h ^ (h >> 13)) * 1274126177u;
  return h ^ (h >> 16);
}

void draw_stars(const Game* game) {
  const float cell = 120.0f;

  // visible world rectangle, from the screen corners
  Vector2 tl = GetScreenToWorld2D((Vector2){ 0, 0 }, game->cam);
  Vector2 br = GetScreenToWorld2D((Vector2){ (float)GetScreenWidth(), (float)GetScreenHeight() }, game->cam);

  int x0 = (int)floorf(tl.x / cell), x1 = (int)floorf(br.x / cell);
  int y0 = (int)floorf(tl.y / cell), y1 = (int)floorf(br.y / cell);

  for (int cy = y0; cy <= y1; cy++) {
    for (int cx = x0; cx <= x1; cx++) {
      uint32_t h = hash2(cx, cy);
      if (h % 4 == 0) continue;

      float ox = (h & 0xFF) / 255.0f;
      float oy = ((h >> 8) & 0xFF) / 255.0f;
      float size = 1.0f + ((h >> 16) & 0x3) * 0.5f;
      unsigned char b = 120 + ((h >> 20) & 0x7F);

      DrawCircleV((Vector2){ (cx + ox) * cell, (cy + oy) * cell }, size, (Color){ b, b, b, 255 });
    }
  }
}
void update(Game* game, float dt) {
  // Paavo movement
  Paavo* p = &game->paavo;
  Vector2 move = {.x = IsKeyDown(KEY_F) - IsKeyDown(KEY_S),
                  .y = IsKeyDown(KEY_D) - IsKeyDown(KEY_E)};
  move = Vector2Normalize(move);
  p->vel = Vector2Scale(move, WALK_SPEED);

  p->pos = Vector2Add(p->pos, Vector2Scale(p->vel, dt));
}

void draw(Game* game) {
  draw_stars(game);
  switch (game->mode) {
    case MODE_FOOT:
      DrawCircleV(game->paavo.pos, 10, SKYBLUE);
      break;
    case MODE_SHIP:
      DrawCircleV(game->ship.pos, 20, PINK);
      break;
  }
}

int main(void) {
  SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE);
  InitWindow(1920, 1080, "PAAVO");
  SetTargetFPS(GetMonitorRefreshRate(0));

  srand(time(NULL));

  Mode mode = MODE_FOOT;
  Ship ship = (Ship) {
    .pos = {0, 0},
    .vel = {0, 0},
    .rot = 0,

  };
  Paavo paavo = (Paavo) {
    .pos = {0, 0},
    .vel = {0, 0},
  };
  Camera2D cam = (Camera2D) { 0 };
  cam.zoom = 1.0f;
  cam.offset = (Vector2){ GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f };
  Game game = (Game) {
    .mode = mode,
    .ship = ship,
    .paavo = paavo,
    .cam = cam
  };

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();


    update(&game, dt);

    game.cam.target = game.paavo.pos;
    game.cam.offset = (Vector2){ GetRenderWidth() / 2.0f,
                                 GetRenderHeight() / 2.0f };



    BeginDrawing();

    BeginMode2D(game.cam);

    ClearBackground(BLACK);
    draw(&game);

    EndMode2D();

    DrawFPS(0, 0);
    paavo_draw_debug(&game.paavo);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
