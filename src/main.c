#include "raylib.h"
#include <raymath.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define DEBUG_FONT 16
#define DEBUG_LINE (DEBUG_FONT + 2)
#define DEBUG_PAD  8

#define WALK_ACCEL      4000.0f
#define WALK_DRAG       10.0f

#define SLIDE_BOOST      450.0f
#define SLIDE_DRAG       0.6f
#define SLIDE_STOP_SPEED 5.0f
#define SLIDE_ACCEL      150.0f

typedef enum { MODE_SHIP, MODE_FOOT } Mode;
typedef struct { Vector2 pos, vel; float rot; } Ship;
typedef struct { Vector2 pos, vel, facing; bool sliding; } Paavo;
typedef struct { Mode mode; Ship ship; Paavo paavo; Camera2D cam; } Game;

void debug_box(Vector2 pos, const char* fmt, ...) {
  char buf[512];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof buf, fmt, args);
  va_end(args);

  // pass 1: split on newlines in place, find line count and widest line
  int lines = 0, max_w = 0;
  for (char* s = buf; s; ) {
    char* nl = strchr(s, '\n');
    if (nl) *nl = '\0';
    int w = MeasureText(s, DEBUG_FONT);
    if (w > max_w) max_w = w;
    lines++;
    s = nl ? nl + 1 : NULL;
  }

  // pass 2: draw the box, then each line
  DrawRectangle(pos.x + 20, pos.y + 20,
                max_w + 2 * DEBUG_PAD,
                lines * DEBUG_LINE + 2 * DEBUG_PAD,
                ColorAlpha(GRAY, 0.2f));

  char* s = buf;
  for (int i = 0; i < lines; i++) {
    DrawText(s, pos.x + DEBUG_PAD + 20, pos.y + DEBUG_PAD + i * DEBUG_LINE + 20, DEBUG_FONT, RAYWHITE);
    s += strlen(s) + 1;
  }
}

void paavo_draw_debug(const Paavo* p) {
  debug_box(p->pos, "x: %.2f\ny: %.2f\nvel: %.1f\nsliding: %s",
            p->pos.x, p->pos.y, Vector2Length(p->vel), p->sliding ? "yes" : "no");
}

void ship_draw_debug(const Ship* s) {
  debug_box(s->pos, "x: %.2f\ny: %.2f\nrot: %.2f", s->pos.x, s->pos.y, s->rot);
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

void paavo_update(Paavo* p, float dt) {
  Vector2 move = Vector2Normalize((Vector2){
      IsKeyDown(KEY_F) - IsKeyDown(KEY_S),
      IsKeyDown(KEY_D) - IsKeyDown(KEY_E) });

  if (Vector2Length(move) > 0) p->facing = move;

  // press: launch forward and start sliding
  if (IsKeyPressed(KEY_SPACE) && !p->sliding) {
    p->vel = Vector2Add(p->vel, Vector2Scale(p->facing, SLIDE_BOOST));
    p->sliding = true;
  }

  // release: back on his feet
  if (p->sliding && !IsKeyDown(KEY_SPACE)) p->sliding = false;

  float accel = p->sliding ? SLIDE_ACCEL : WALK_ACCEL;
  float drag  = p->sliding ? SLIDE_DRAG  : WALK_DRAG;

  p->vel = Vector2Add(p->vel, Vector2Scale(move, accel * dt));
  p->vel = Vector2Scale(p->vel, expf(-drag * dt));
  if (Vector2Length(p->vel) < SLIDE_STOP_SPEED) p->vel = (Vector2){ 0, 0 };

  p->pos = Vector2Add(p->pos, Vector2Scale(p->vel, dt));
}

void ship_update(Ship* s, float dt) {
  (void)s; (void)dt;
}


void update(Game* game, float dt) {
  if (IsKeyPressed(KEY_R)) {
    game->mode = (game->mode == MODE_FOOT) ? MODE_SHIP : MODE_FOOT;

    game->paavo.vel = (Vector2){ 0, 0 };
    game->paavo.sliding = false;
  }

  switch (game->mode) {
    case MODE_FOOT: paavo_update(&game->paavo, dt); break;
    case MODE_SHIP: ship_update(&game->ship, dt);    break;
  }

  Vector2 goal = (game->mode == MODE_FOOT) ? game->paavo.pos : game->ship.pos;
  game->cam.target = Vector2Lerp(game->cam.target, goal, 1.0f - expf(-10.0f * dt));
  game->cam.offset = (Vector2){ GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f };
}

void draw(Game* game) {
  draw_stars(game);
  if (game->paavo.sliding) {
    DrawCircleLinesV(game->paavo.pos, 11, RAYWHITE);
  }

  DrawCircleV(game->paavo.pos, 10, SKYBLUE);
  DrawCircleV(game->ship.pos, 20, PINK);
}

int main(void) {
  SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE);
  InitWindow(1920, 1080, "PAAVO");
  SetTargetFPS(GetMonitorRefreshRate(0));

  srand('h' + 'i');

  Mode mode = MODE_FOOT;
  Ship ship = (Ship) {
    .pos = {0, 0},
    .vel = {0, 0},
    .rot = 0,

  };
  Paavo paavo = (Paavo) {
    .pos = {0, 0},
    .vel = {0, 0},
    .facing = {1, 0},
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

    BeginDrawing();

    BeginMode2D(game.cam);

    ClearBackground(BLACK);
    draw(&game);
    paavo_draw_debug(&game.paavo);
    ship_draw_debug(&game.ship);

    EndMode2D();

    DrawFPS(0, 0);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
