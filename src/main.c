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

// items
#define MAX_ITEMS 100
#define INV_SIZE 30

// inventory drawing
#define SLOT_SIZE 56
#define SLOT_GAP  6
#define INV_COLS  10

typedef struct {
  Vector2 pos, vel;
  char* name;
  int count;
} Item;

typedef struct {
  Vector2 pos, vel, facing;
  float heading;
  bool sliding;
  Item *inv[INV_SIZE];
} Paavo;

typedef struct { Paavo paavo;
  Camera2D cam;
  Item *items[MAX_ITEMS];
} Game;

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
  debug_box(p->pos, "Paavo\nx: %.2f\ny: %.2f\nspd: %.1f\nsliding: %s",
            p->pos.x, p->pos.y, Vector2Length(p->vel), p->sliding ? "yes" : "no");
}

void item_draw_debug(const Item* i) {
  if (i == NULL) return;
  debug_box(i->pos, "%s\nx: %.2f\ny: %.2f\ncount: %d",
            i->name, i->pos.x, i->pos.y, i->count);
}

void item_draw_icon(const char* name, Vector2 pos) {
  float x = pos.x, y = pos.y;

  if (strcmp(name, "fish") == 0) {
    DrawEllipse((int)x, (int)y, 10, 6, GRAY);
    DrawTriangle(
        (Vector2){ x - 16, y - 6 },
        (Vector2){ x - 16, y + 6 },
        (Vector2){ x - 6,  y },
        GRAY);
  } else if (strcmp(name, "cheese") == 0) {
  Color holes = (Color){ 214, 160, 0, 255 };

  DrawTriangle(
      (Vector2){ x + 12, y - 8 },
      (Vector2){ x - 12, y + 8 },
      (Vector2){ x + 12, y + 8 },
      YELLOW);

  DrawCircleV((Vector2){ x + 7, y + 2 }, 2.5f, holes);
  DrawCircleV((Vector2){ x + 1, y + 4 }, 1.5f, holes);
  DrawCircleV((Vector2){ x + 9, y - 2 }, 1.5f, holes);

  DrawTriangleLines(
      (Vector2){ x + 12, y - 8 },
      (Vector2){ x - 12, y + 8 },
      (Vector2){ x + 12, y + 8 },
      holes);  } else if (strcmp(name, "snowball") == 0) {
    DrawCircleV(pos, 5, RAYWHITE);

  } else if (strcmp(name, "apple") == 0) {
    DrawCircleV(pos, 8, RED);
    DrawRectangle(x - 1, y - 12, 2, 5, BROWN);
    DrawEllipse(x + 4, y - 10, 4, 2, GREEN);

  } else if (strcmp(name, "coin") == 0) {
    DrawCircleV(pos, 8, GOLD);
    DrawCircleLinesV(pos, 5, ORANGE);

  } else if (strcmp(name, "key") == 0) {
    DrawRing((Vector2){ x - 9, y }, 2, 5, 0, 360, 16, GOLD);
    DrawRectangle(x - 4, y - 1, 16, 3, GOLD);
    DrawRectangle(x + 6, y + 2, 2, 4, GOLD);
    DrawRectangle(x + 10, y + 2, 2, 3, GOLD);

  } else if (strcmp(name, "gem") == 0) {
    DrawPoly(pos, 4, 10, 0, PURPLE);
    DrawPoly(pos, 4, 4, 0, ColorAlpha(WHITE, 0.5f));

  } else if (strcmp(name, "mushroom") == 0) {
    DrawRectangle(x - 3, y - 1, 6, 9, BEIGE);
    DrawCircleSector((Vector2){ x, y - 1 }, 10, 180, 360, 16, RED);
    DrawCircle(x - 4, y - 5, 2, RAYWHITE);
    DrawCircle(x + 3, y - 7, 2, RAYWHITE);

  } else if (strcmp(name, "potion") == 0) {
    DrawCircleV((Vector2){ x, y + 3 }, 9, LIME);
    DrawRectangle(x - 3, y - 9, 6, 6, LIGHTGRAY);
    DrawRectangle(x - 4, y - 12, 8, 3, BROWN);
    DrawCircle(x - 3, y + 1, 2, ColorAlpha(WHITE, 0.6f));

  } else if (strcmp(name, "log") == 0) {
    DrawRectangle(x - 12, y - 6, 24, 12, BROWN);
    DrawEllipse(x + 12, y, 4, 6, BEIGE);
    DrawEllipseLines(x + 12, y, 2, 3, BROWN);
  }
}
void inv_draw(Item* const inv[]) {
  int rows = (INV_SIZE + INV_COLS - 1) / INV_COLS;
  int w = INV_COLS * SLOT_SIZE + (INV_COLS - 1) * SLOT_GAP;
  int h = rows * SLOT_SIZE + (rows - 1) * SLOT_GAP;
  int x0 = (GetScreenWidth() - w) / 2;
  int y0 = GetScreenHeight() - h - 20;

  for (int i = 0; i < INV_SIZE; i++) {
    int col = i % INV_COLS, row = i / INV_COLS;
    Rectangle r = {
      x0 + col * (SLOT_SIZE + SLOT_GAP),
      y0 + row * (SLOT_SIZE + SLOT_GAP),
      SLOT_SIZE, SLOT_SIZE
    };

    DrawRectangleRec(r, ColorAlpha(GRAY, 0.2f));
    DrawRectangleLinesEx(r, 2, ColorAlpha(RAYWHITE, 0.4f));

    if (inv[i] == NULL) continue;

    item_draw_icon(inv[i]->name,
                   (Vector2){ r.x + r.width / 2, r.y + r.height / 2 - 4 });

    const char* txt = TextFormat("%d", inv[i]->count);
    int tw = MeasureText(txt, 14);
    DrawText(txt, r.x + r.width - tw - 4, r.y + r.height - 16, 14, RAYWHITE);
  }
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

int spawn_item(Game* g, Vector2 pos, char* name, int count) {
  for (int i = 0; i < MAX_ITEMS; i++)  {
    if (g->items[i] == NULL) {
      Item* item = calloc(1, sizeof(Item));
      item->pos = pos;
      item->name = malloc(sizeof(char) * (strlen(name) + 1));
      strcpy(item->name, name);
      item->count = count;
      g->items[i] = item;
      return 1;
    }
  }
  return 0;
}

int paavo_inv_add(Paavo* p, Item* item) {
  for (int i = 0; i < INV_SIZE; i++) {
    if (p->inv[i] == NULL) continue;
    if (strcmp(item->name, p->inv[i]->name) == 0) {
      p->inv[i]->count += item->count;
      free(item->name);
      free(item);
      return 1;
    }
  }
  for (int i = 0; i < INV_SIZE; i++) {
    if (p->inv[i] == NULL) {
      p->inv[i] = item;
      return 1;
    }
  }
  return 0;
}

bool inv_full(Item* const inv[]) {
  for (int i = 0; i < INV_SIZE; i++) {
    if (inv[i] == NULL) return false;
  }
  return true;
}

bool inv_can_add(Item* const inv[], const Item* item) {
  for (int i = 0; i < INV_SIZE; i++) {
    if (inv[i] == NULL) return true;
    if (strcmp(inv[i]->name, item->name) == 0) return true;
  }
  return false;
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

  for (int i = 0; i < MAX_ITEMS; i++) {
    Item* item = game->items[i];
    if ((item == NULL) || !inv_can_add(game->paavo.inv, item)) continue;

    float dist = Vector2Distance(game->paavo.pos, item->pos);
    if (dist < 80 && dist > 0.001f) {
        Vector2 dir = Vector2Normalize(Vector2Subtract(game->paavo.pos, item->pos));
        item->vel = Vector2Add(item->vel, Vector2Scale(dir, 30.0f));
    }

    item->vel = Vector2Scale(item->vel, 0.95f);

    item->pos = Vector2Add(item->pos, Vector2Scale(item->vel, dt));

    if (Vector2Distance(item->pos, game->paavo.pos) < 10) {
      if (paavo_inv_add(&game->paavo, item)) game->items[i] = NULL;
    }
  }
}

void draw(Game* game) {
  draw_stars(game);
  if (game->paavo.sliding) {
    DrawCircleLinesV(game->paavo.pos, 11, RAYWHITE);
  }

  DrawCircleV(game->paavo.pos, 10, SKYBLUE);


  for (int i = 0; i < MAX_ITEMS; i++) {
    if (game->items[i] == NULL) continue;
    item_draw_icon(game->items[i]->name, game->items[i]->pos);
  }
}

int main(void) {
  SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE);
  InitWindow(1920, 1080, "PAAVO");
  SetTargetFPS(GetMonitorRefreshRate(1));

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
  spawn_item(&game, (Vector2){100, 500}, "fish", 11);
  spawn_item(&game, (Vector2){100, 600}, "cheese", 15);
  spawn_item(&game, (Vector2){200, 500}, "snowball", 16);
  spawn_item(&game, (Vector2){200, 600}, "apple", 5);
  spawn_item(&game, (Vector2){300, 600}, "coin", 25);
  spawn_item(&game, (Vector2){300, 500}, "key", 1);
  spawn_item(&game, (Vector2){400, 600}, "gem", 3);
  spawn_item(&game, (Vector2){400, 500}, "mushroom", 8);
  spawn_item(&game, (Vector2){500, 600}, "potion", 2);
  spawn_item(&game, (Vector2){500, 500}, "log", 10);

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    update(&game, dt);

    BeginDrawing();

    BeginMode2D(game.cam);

    ClearBackground(BLACK);
    draw(&game);
    if (DEBUG) {
      paavo_draw_debug(&game.paavo);

      for (int i = 0; i < MAX_ITEMS; i++) {
        item_draw_debug(game.items[i]);
      }
    }

    EndMode2D();

    inv_draw(game.paavo.inv);

    DrawFPS(0, 0);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
