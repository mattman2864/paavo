#include <raylib.h>
#include <raymath.h>
#include "penguin.h"

void drawPenguin(Penguin* Penguin) {
  DrawRectangle(
      Penguin->x, Penguin->y,
      Penguin->width, Penguin->height,
      RAYWHITE);
}

void movePenguin(Penguin *penguin, Vector2 d, float dt) {
  d = Vector2Normalize(d);
  d = Vector2Scale(d, 300.f);
  penguin->x += d.x * dt;
  penguin->y += d.y * dt;
}
