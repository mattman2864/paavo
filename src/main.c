#include "raylib.h"
#include <raymath.h>

typedef enum { MODE_SHIP, MODE_FOOT } Mode;
typedef struct { Vector2 pos, vel; float rot; } Ship;
typedef struct { Vector2 pos, vel; } Paavo;
typedef struct { Mode mode; Ship ship; Paavo paavo; Camera2D cam; } Game;

void update(Game* game, float dt) {
  return;
}

void draw(Game* game) {
  DrawCircle(game->paavo.pos.x, game->paavo.pos.y, 10, SKYBLUE);
  return;
}

int main(void) {
  SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE);
  InitWindow(1920, 1080, "PAAVO");
  SetTargetFPS(GetMonitorRefreshRate(0));

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



    BeginDrawing();

      BeginMode2D(game.cam);

        ClearBackground(BLACK);
        draw(&game);

      EndMode2D();

      DrawFPS(0, 0);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
