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

// util
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// debug
#define DEBUG true
#define DEBUG_FONT 16
#define DEBUG_LINE (DEBUG_FONT + 2)
#define DEBUG_PAD  8

// paavo movement
#define WALK_ACCEL          5000.0f
#define WALK_DRAG           15.0f
#define SLIDE_BOOST         2.0f
#define SLIDE_DRAG          0.6f
#define SLIDE_TURN_RATE     0.5f
#define SLIDE_STOP_SPEED    5.0f
#define SLIDE_RELEASE_SPEED 400.0f

// items
#define MAX_ITEMS 100
#define INV_SIZE 30
#define MAGNET_RADIUS 80.0f
#define MAGNET_ACCEL  1800.0f
#define ITEM_DRAG     3.0f

// inventory drawing
#define SLOT_SIZE 56
#define SLOT_GAP  6
#define INV_COLS  10

// tiles
#define TILE_SIZE 32


typedef enum { ITEM_FISH, ITEM_CHEESE, ITEM_SNOWBALL, ITEM_APPLE, ITEM_COIN,
  ITEM_KEY, ITEM_GEM, ITEM_MUSHROOM, ITEM_POTION, ITEM_LOG,
  ITEM_COUNT } ItemType;

typedef struct { const char* name; int max_stack; } ItemDef;

static const ItemDef ITEM_DEFS[ITEM_COUNT] = {
  [ITEM_FISH]     = { "fish",     99 },
  [ITEM_CHEESE]   = { "cheese",   99 },
  [ITEM_SNOWBALL] = { "snowball", 16 },
  [ITEM_APPLE]    = { "apple",    99 },
  [ITEM_COIN]     = { "coin",    999 },
  [ITEM_KEY]      = { "key",       1 },
  [ITEM_GEM]      = { "gem",      99 },
  [ITEM_MUSHROOM] = { "mushroom", 99 },
  [ITEM_POTION]   = { "potion",   16 },
  [ITEM_LOG]      = { "log",      99 },
};

typedef enum { TILE_WATER, TILE_ICE, TILE_SNOW, TILE_ROCK, TILE_COUNT } TileType;

typedef struct { const char* name; float drag_multiplier; bool solid; Color color; } TileDef;

static const TileDef TILE_DEFS[TILE_COUNT] = {

  [TILE_WATER] = { "water", 1.0f, true,  { 30, 80, 160, 255 } },
  [TILE_ICE]   = { "ice",   0.3f, false, { 170, 220, 240, 255 } },
  [TILE_SNOW]  = { "snow",  1.0f, false, { 235, 240, 245, 255 } },
  [TILE_ROCK]  = { "rock",  1.0f, true,  { 90, 90, 100, 255 } },

};

typedef struct { ItemType type; int count; } ItemStack;
typedef struct { Vector2 pos, vel; ItemStack stack; bool active; } WorldItem;

typedef struct {
  Vector2 pos, vel, facing;
  float heading;
  bool sliding;
  ItemStack inv[INV_SIZE];
} Paavo;

typedef struct { Paavo paavo;
  Camera2D cam;
  WorldItem items[MAX_ITEMS];
  uint32_t seed;
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

void item_draw_debug(const WorldItem* it) {
  if (!it->active) return;
  debug_box(it->pos, "%s\nx: %.2f\ny: %.2f\ncount: %d",
      ITEM_DEFS[it->stack.type].name, it->pos.x, it->pos.y, it->stack.count);
}


void item_draw_icon(ItemType type, Vector2 pos) {
  float x = pos.x, y = pos.y;

  switch (type) {
    case ITEM_FISH:
      DrawEllipse(x, y, 10, 6, GRAY);
      DrawTriangle((Vector2){ x - 16, y - 6 }, (Vector2){ x - 16, y + 6 },
          (Vector2){ x - 6, y }, GRAY);
      break;

    case ITEM_CHEESE: {
                        Color holes = (Color){ 214, 160, 0, 255 };
                        Vector2 a = { x + 12, y - 8 }, b = { x - 12, y + 8 }, c = { x + 12, y + 8 };
                        DrawTriangle(a, b, c, YELLOW);
                        DrawCircleV((Vector2){ x + 7, y + 2 }, 2.5f, holes);
                        DrawCircleV((Vector2){ x + 1, y + 4 }, 1.5f, holes);
                        DrawCircleV((Vector2){ x + 9, y - 2 }, 1.5f, holes);
                        DrawTriangleLines(a, b, c, holes);
                      } break;

    case ITEM_SNOWBALL:
                      DrawCircleV(pos, 5, RAYWHITE);
                      break;

    case ITEM_APPLE:
                      DrawCircleV(pos, 8, RED);
                      DrawRectangle(x - 1, y - 12, 2, 5, BROWN);
                      DrawEllipse(x + 4, y - 10, 4, 2, GREEN);
                      break;

    case ITEM_COIN:
                      DrawCircleV(pos, 8, GOLD);
                      DrawCircleLinesV(pos, 5, ORANGE);
                      break;

    case ITEM_KEY:
                      DrawRing((Vector2){ x - 9, y }, 2, 5, 0, 360, 16, GOLD);
                      DrawRectangle(x - 4, y - 1, 16, 3, GOLD);
                      DrawRectangle(x + 6, y + 2, 2, 4, GOLD);
                      DrawRectangle(x + 10, y + 2, 2, 3, GOLD);
                      break;

    case ITEM_GEM:
                      DrawPoly(pos, 4, 10, 0, PURPLE);
                      DrawPoly(pos, 4, 4, 0, ColorAlpha(WHITE, 0.5f));
                      break;

    case ITEM_MUSHROOM:
                      DrawRectangle(x - 3, y - 1, 6, 9, BEIGE);
                      DrawCircleSector((Vector2){ x, y - 1 }, 10, 180, 360, 16, RED);
                      DrawCircle(x - 4, y - 5, 2, RAYWHITE);
                      DrawCircle(x + 3, y - 7, 2, RAYWHITE);
                      break;

    case ITEM_POTION:
                      DrawCircleV((Vector2){ x, y + 3 }, 9, LIME);
                      DrawRectangle(x - 3, y - 9, 6, 6, LIGHTGRAY);
                      DrawRectangle(x - 4, y - 12, 8, 3, BROWN);
                      DrawCircle(x - 3, y + 1, 2, ColorAlpha(WHITE, 0.6f));
                      break;

    case ITEM_LOG:
                      DrawRectangle(x - 12, y - 6, 24, 12, BROWN);
                      DrawEllipse(x + 12, y, 4, 6, BEIGE);
                      DrawEllipseLines(x + 12, y, 2, 3, BROWN);
                      break;

    case ITEM_COUNT: break;
  }
}

void inv_draw(const ItemStack inv[]) {
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

    if (inv[i].count == 0) continue;

    item_draw_icon(inv[i].type,
        (Vector2){ r.x + r.width / 2, r.y + r.height / 2 - 4 });

    const char* txt = TextFormat("%d", inv[i].count);
    int tw = MeasureText(txt, 14);
    DrawText(txt, r.x + r.width - tw - 4, r.y + r.height - 16, 14, RAYWHITE);
  }
}

bool spawn_item(Game* g, Vector2 pos, ItemType type, int count) {
  for (int i = 0; i < MAX_ITEMS; i++) {
    if (!g->items[i].active) {
      g->items[i] = (WorldItem){
        .pos = pos,
        .stack = { type, count },
        .active = true,
      };
      return true;
    }
  }
  return false;
}

// can at ast one more of this type fit?
bool inv_can_add(const ItemStack inv[], ItemType type) {
  int max = ITEM_DEFS[type].max_stack;
  for (int i = 0; i < INV_SIZE; i++) {
    if (inv[i].count == 0) return true;
    if (inv[i].type == type && inv[i].count < max) return true;
  }
  return false;
}

static uint32_t hash2(int x, int y) {
  uint32_t h = (uint32_t)x * 374761393u + (uint32_t)y * 668265263u;
  h = (h ^ (h >> 13)) * 1274126177u;
  return h ^ (h >> 16);
}

static uint32_t hash3(int x, int y, uint32_t seed) {
  uint32_t h = (uint32_t)x * 374761393u + (uint32_t)y * 668265263u + seed * 2246822519u;
  h = (h ^ (h >> 13)) * 1274126177u;
  return h ^ (h >> 16);
}

static float lattice(int x, int y, uint32_t seed) {
  return (hash3(x, y, seed) & 0xFFFF) / 65535.0f;
}

static float noise2(float x, float y, uint32_t seed) {
  int xi = (int)floorf(x), yi = (int)floorf(y);
  float fx = x - xi, fy = y - yi;
  float u = fx * fx * (3 - 2 * fx), v = fy * fy * (3 - 2 * fy);   // smoothstep
  float a = lattice(xi,     yi,     seed), b = lattice(xi + 1, yi,     seed);
  float c = lattice(xi,     yi + 1, seed), d = lattice(xi + 1, yi + 1, seed);
  return Lerp(Lerp(a, b, u), Lerp(c, d, u), v);
}

static float fbm(float x, float y, uint32_t seed) {
  float sum = 0, amp = 0.5f;
  for (int i = 0; i < 4; i++) {
    sum += amp * noise2(x, y, seed + i);
    x *= 2; y *= 2; amp *= 0.5f;
  }
  return sum / 0.9375f;
}

TileType tile_at(int tx, int ty, uint32_t seed) {
  float e = fbm(tx * 0.04f, ty * 0.04f, seed);
  if (e < 0.30f) return TILE_WATER;
  if (e < 0.42f) return TILE_ICE;
  if (e < 0.72f) return TILE_SNOW;
  return TILE_ROCK;
}

void draw_tiles(const Game* g) {
  Vector2 tl = GetScreenToWorld2D((Vector2){ 0, 0 }, g->cam);
  Vector2 br = GetScreenToWorld2D((Vector2){ (float)GetScreenWidth(), (float)GetScreenHeight() }, g->cam);
  int x0 = (int)floorf(tl.x / TILE_SIZE), x1 = (int)floorf(br.x / TILE_SIZE);
  int y0 = (int)floorf(tl.y / TILE_SIZE), y1 = (int)floorf(br.y / TILE_SIZE);

  for (int ty = y0; ty <= y1; ty++)
    for (int tx = x0; tx <= x1; tx++)
      DrawRectangle(tx * TILE_SIZE, ty * TILE_SIZE, TILE_SIZE, TILE_SIZE,
          TILE_DEFS[tile_at(tx, ty, g->seed)].color);
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

static float ang_diff(float a, float b) {
  float d = fmodf(b - a + PI, 2.0f * PI);
  if (d < 0) d += 2.0f * PI;
  return d - PI;
}

int inv_add(ItemStack inv[], ItemType type, int count) {
  int max = ITEM_DEFS[type].max_stack;
  for (int i = 0; i < INV_SIZE && count > 0; i++) {
    if (inv[i].count > 0 && inv[i].type == type && inv[i].count < max) {
      int n = MIN(count, max - inv[i].count);
      inv[i].count += n;  count -= n;
    }
  }
  for (int i = 0; i < INV_SIZE && count > 0; i++) {
    if (inv[i].count == 0) {
      int n = MIN(count, max);
      inv[i] = (ItemStack){ type, n };  count -= n;
    }
  }
  return count;
}

void paavo_update(Paavo* p, uint32_t seed, float dt) {
  Vector2 move = Vector2Normalize((Vector2) {
      IsKeyDown(KEY_F) - IsKeyDown(KEY_S),
      IsKeyDown(KEY_D) - IsKeyDown(KEY_E)});
  bool has_input = Vector2Length(move) > 0;
  if (has_input) p->facing = move;


  TileType under = tile_at((int)floorf(p->pos.x / TILE_SIZE),
      (int)floorf(p->pos.y / TILE_SIZE), seed);
  float grip = TILE_DEFS[under].drag_multiplier;

  if (IsKeyPressed(KEY_SPACE) && !p->sliding) {
    p->heading = atan2f(p->facing.y, p->facing.x);
    p->vel = Vector2Scale(p->facing, SLIDE_BOOST * Vector2Length(p->vel));
    p->sliding = true;
  }

  if (p->sliding && !IsKeyDown(KEY_SPACE)
      && Vector2Length(p->vel) <= SLIDE_RELEASE_SPEED) p->sliding = false;

  if (p->sliding) {
    if (has_input) {
      float target = atan2f(move.y, move.x);
      float diff   = ang_diff(p->heading, target);
      float max_step = SLIDE_TURN_RATE * dt;
      if (diff >  max_step) diff =  max_step;
      if (diff < -max_step) diff = -max_step;
      p->heading += diff;
    }

    float speed = Vector2Length(p->vel) * expf(-SLIDE_DRAG * grip * dt);
    if (speed < SLIDE_STOP_SPEED) speed = 0;
    p->vel = (Vector2){ cosf(p->heading) * speed, sinf(p->heading) * speed };
  } else {
    p->vel = Vector2Add(p->vel, Vector2Scale(move, WALK_ACCEL * grip * dt));
    p->vel = Vector2Scale(p->vel, expf(-WALK_DRAG * grip * dt));
  }

  p->pos = Vector2Add(p->pos, Vector2Scale(p->vel, dt));
}

void update(Game* game, float dt) {
  paavo_update(&game->paavo, game->seed, dt);

  Vector2 goal = game->paavo.pos;
  game->cam.target = Vector2Lerp(game->cam.target, goal, 1.0f - expf(-10.0f * dt));
  game->cam.offset = (Vector2){ GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f };

  for (int i = 0; i < MAX_ITEMS; i++) {
    WorldItem* it = &game->items[i];
    if (!it->active) continue;

    float dist = Vector2Distance(game->paavo.pos, it->pos);
    bool can_pick = inv_can_add(game->paavo.inv, it->stack.type);

    // only items Paavo has room for get puld toward him
    if (can_pick && dist < MAGNET_RADIUS && dist > 0.001f) {
      Vector2 dir = Vector2Normalize(Vector2Subtract(game->paavo.pos, it->pos));
      it->vel = Vector2Add(it->vel, Vector2Scale(dir, MAGNET_ACCEL * dt));
    }

    it->vel = Vector2Scale(it->vel, expf(-ITEM_DRAG * dt));
    it->pos = Vector2Add(it->pos, Vector2Scale(it->vel, dt));

    if (can_pick && dist < 10) {
      // inv_add returns how many did NOT fit; those stay on the ground
      it->stack.count = inv_add(game->paavo.inv, it->stack.type, it->stack.count);
      if (it->stack.count == 0) it->active = false;
    }
  }
}

void draw(Game* game) {
  draw_stars(game);
  draw_tiles(game);
  if (game->paavo.sliding) {
    DrawCircleLinesV(game->paavo.pos, 11, RAYWHITE);
  }

  DrawCircleV(game->paavo.pos, 10, SKYBLUE);


  for (int i = 0; i < MAX_ITEMS; i++) {
    if (!game->items[i].active) continue;
    item_draw_icon(game->items[i].stack.type, game->items[i].pos);
  }
}

int main(void) {
  SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE);
  InitWindow(1920, 1080, "PAAVO");
  SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

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
    .cam = cam,
    .seed = 1337
  };
  spawn_item(&game, (Vector2){100, 500}, ITEM_FISH,     11);
  spawn_item(&game, (Vector2){100, 600}, ITEM_CHEESE,   15);
  spawn_item(&game, (Vector2){200, 500}, ITEM_SNOWBALL, 24);
  spawn_item(&game, (Vector2){200, 600}, ITEM_APPLE,     5);
  spawn_item(&game, (Vector2){300, 600}, ITEM_COIN,   5344);
  spawn_item(&game, (Vector2){300, 500}, ITEM_KEY,       1);
  spawn_item(&game, (Vector2){400, 600}, ITEM_GEM,       3);
  spawn_item(&game, (Vector2){400, 500}, ITEM_MUSHROOM,  8);
  spawn_item(&game, (Vector2){500, 600}, ITEM_POTION,    2);
  spawn_item(&game, (Vector2){500, 500}, ITEM_LOG,      10);

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
        item_draw_debug(&game.items[i]);
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
