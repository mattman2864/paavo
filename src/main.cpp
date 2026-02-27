#include "Body.h"
#include "Control.h"
#include "Sprite.h"
#include "raylib.h"
#include <cmath>
#include <ctime>
#include <raymath.h>

#include "Entity.h"

float scale = 4;

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(800, 600, "PAAVO");
  MaximizeWindow();
  SetTargetFPS(GetMonitorRefreshRate(0));

  Entity *player = new Entity(
      new KeyboardControl(DEFAULT_KB),
      new Body({0, 0}, {.x = -16, .y = -32, .width = 32, .height = 64}),
      new Sprite("assets/entities/player/player.png")
      );

  Entity *enemy = new Entity(
      new WalkRightControl(),
      new Body({0, 0}, {.x = -8, .y = -8, .width = 16, .height = 16}),
      new Sprite("assets/entities/star/star.png")
      );


  Camera2D cam = {
    .offset = {-(float)GetMonitorWidth(0)/2, -(float)GetMonitorHeight(0)/2},
    .target = player->getBody()->getPosition(),
    .rotation = 0,
    .zoom = 2,
  };

  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();

    player->update(deltaTime);
    enemy->update(deltaTime);

    cam.offset = {(float)GetScreenWidth()/2, (float)GetScreenHeight()/2};
    cam.target = player->getBody()->getPosition();

    BeginDrawing();
    ClearBackground({.r = 200, .g = 200, .b = 200});

    BeginMode2D(cam);

    player->draw();
    enemy->draw();

    EndMode2D();

    DrawFPS(5, 5);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
