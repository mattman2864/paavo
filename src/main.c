#include "raylib.h"
#include <raymath.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// debug
#define DEBUG true
#define DEBUG_FONT 16
#define DEBUG_LINE (DEBUG_FONT + 2)
#define DEBUG_PAD  8

// paavo movement
#define WALK_ACCEL       5000.0f
#define WALK_DRAG        15.0f
#define SLIDE_BOOST      2.0f
#define SLIDE_DRAG       0.6f
#define SLIDE_TURN_RATE  0.5f
#define SLIDE_STOP_SPEED 5.0f

typedef struct { Vector2 pos, vel, facing; float heading; bool sliding; } Paavo;
typedef struct { Paavo paavo; Camera2D cam; } Game;

void debug_box(Vector2 pos, const char* fmt, ...) {
  char buf[512];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof buf, fmt, args);
  va_end(args);

  int lines = 0, max_w = 0;
  for (char* s = buf; s; ) {
    char* nl = strchr(s, '\n');
    if (nl) *nl = '\0';
    int w = MeasureText(s, DEBUG_FONT);
    if (w > max_w) max_w = w;
    lines++;
    s = nl ? nl + 1 : NULL;
  }

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
  debug_box(p->pos, "x: %.2f\ny: %.2f\nspd: %.1f\nsliding: %s",
            p->pos.x, p->pos.y, Vector2Length(p->vel), p->sliding ? "yes" : "no");
}

static uint32_t hash2(int x, int y) {
  uint32_t h = (uint32_t)x * 374761393u + (uint32_t)y * 668265263u;
  h = (h ^ (h >> 13)) * 1274126177u;
  return h ^ (h >> 16);
}

void draw_stars(const Game* game) {
  const float cell = 120.0f;

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

static float angle_diff(float a, float b) {
  float d = fmodf(b - a + PI, 2.0f * PI);
  if (d < 0) d += 2.0f * PI;
  return d - PI;
}

void paavo_update(Paavo* p, float dt) {
  Vector2 move = Vector2Normalize((Vector2) {
      IsKeyDown(KEY_F) - IsKeyDown(KEY_S),
      IsKeyDown(KEY_D) - IsKeyDown(KEY_E)});
  bool has_input = Vector2Length(move) > 0;
  if (has_input) p->facing = move;

  if (IsKeyPressed(KEY_SPACE) && !p->sliding) {
    p->heading = atan2f(p->facing.y, p->facing.x);
    p->vel = Vector2Scale(p->facing, SLIDE_BOOST * Vector2Length(p->vel));
    p->sliding = true;
  }

  if (p->sliding && !IsKeyDown(KEY_SPACE)) p->sliding = false;

  if (p->sliding) {
    if (has_input) {
      float target = atan2f(move.y, move.x);
      float diff   = angle_diff(p->heading, target);
      float max_step = SLIDE_TURN_RATE * dt;
      if (diff >  max_step) diff =  max_step;
      if (diff < -max_step) diff = -max_step;
      p->heading += diff;
    }

    float speed = Vector2Length(p->vel) * expf(-SLIDE_DRAG * dt);
    if (speed < SLIDE_STOP_SPEED) speed = 0;
    p->vel = (Vector2){ cosf(p->heading) * speed, sinf(p->heading) * speed };
  } else {
    p->vel = Vector2Add(p->vel, Vector2Scale(move, WALK_ACCEL * dt));
    p->vel = Vector2Scale(p->vel, expf(-WALK_DRAG * dt));
  }

  p->pos = Vector2Add(p->pos, Vector2Scale(p->vel, dt));
}

void update(Game* game, float dt) {
  paavo_update(&game->paavo, dt);

  Vector2 goal = game->paavo.pos;
  game->cam.target = Vector2Lerp(game->cam.target, goal, 1.0f - expf(-10.0f * dt));
  game->cam.offset = (Vector2){ GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f };
}

void draw(Game* game) {
  draw_stars(game);
  if (game->paavo.sliding) {
    DrawCircleLinesV(game->paavo.pos, 11, RAYWHITE);
  }

  DrawCircleV(game->paavo.pos, 10, SKYBLUE);
}

int main(void) {
  SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE);
  InitWindow(1920, 1080, "PAAVO");
  SetTargetFPS(GetMonitorRefreshRate(0));

  srand(time(NULL));

  Paavo paavo = (Paavo) {
    .pos = {0, 0},
    .vel = {0, 0},
    .facing = {1, 0},
  };
  Camera2D cam = (Camera2D) { 0 };
  cam.zoom = 1.0f;
  cam.offset = (Vector2){ GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f };
  Game game = (Game) {
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
    if (DEBUG) {
      paavo_draw_debug(&game.paavo);
    }

    EndMode2D();

    DrawFPS(0, 0);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
