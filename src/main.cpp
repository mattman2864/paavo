#include <raylib.h>
#include "player.cpp"
#include "object.cpp"

int main()
{
  InitWindow(1200, 800, "PAAVO");

  Player p = Player(100, 100);
  Object o = Object("assets/objects/rocks/rock.png", {200, 200}, {100, 100});

  while (!WindowShouldClose())
  {
    float dt = GetFrameTime();

    Vector2 dir = {(float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A)),
                  (float)(IsKeyDown(KEY_S) - IsKeyDown(KEY_W))};
    p.move(dir, dt);
    if (IsKeyPressed(KEY_SPACE)) {
      p.dash(dir);
    }
    p.update(dt);

    // Draw
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawFPS(2, 0);
    p.draw();
    o.draw();

    EndDrawing();
  }
  p.unload();
  o.unload();

  return 0;
}
