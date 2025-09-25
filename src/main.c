#include "raylib.h"
#include "raymath.h"
#include "math.h"
#include <stdlib.h>
#include <raylib.h>
#include "RectangleList.c"
#include "Player.c"

int main(void)
{
  InitWindow(800, 800, "paavo");

  SetTargetFPS(144);
  Player player = {.pos = {50, 50}, .walkingSpeed = 200, .size = 20};

  RectangleList collisionObjects;
  InitRectangleList(&collisionObjects, 16);
 
  AddRectangle(&collisionObjects, (Rectangle){0, 0, 780, 20});
  AddRectangle(&collisionObjects, (Rectangle){780, 0, 20, 780});
  AddRectangle(&collisionObjects, (Rectangle){0, 20, 20, 780});
  AddRectangle(&collisionObjects, (Rectangle){20, 780, 780, 20});
 
  AddRectangle(&collisionObjects, (Rectangle){100, 100, 580, 20});
  AddRectangle(&collisionObjects, (Rectangle){680, 100, 20, 580});
  AddRectangle(&collisionObjects, (Rectangle){100, 120, 20, 580});
  AddRectangle(&collisionObjects, (Rectangle){120, 680, 580, 20});

  AddRectangle(&collisionObjects, (Rectangle){200, 200, 380, 20});
  AddRectangle(&collisionObjects, (Rectangle){580, 200, 20, 380});
  AddRectangle(&collisionObjects, (Rectangle){200, 220, 20, 380});
  AddRectangle(&collisionObjects, (Rectangle){220, 580, 380, 20});

  AddRectangle(&collisionObjects, (Rectangle){300, 300, 180, 20});
  AddRectangle(&collisionObjects, (Rectangle){480, 300, 20, 180});
  AddRectangle(&collisionObjects, (Rectangle){300, 320, 20, 180});
  AddRectangle(&collisionObjects, (Rectangle){320, 480, 180, 20});

  while (!WindowShouldClose())
  {
    float dt = GetFrameTime();
    // Update
    Player_move(&player, (Vector2){((int)IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) - (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)),
                                   ((int)IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) - (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))}, dt, &collisionObjects);

    // Draw
    BeginDrawing();
      ClearBackground(RAYWHITE);
      Player_draw(&player);
      DrawAllRectangles(&collisionObjects);
      DrawFPS(10, 10);
    EndDrawing();
  }

  ClearRectangleList(&collisionObjects);
  CloseWindow();

  return 0;
}
