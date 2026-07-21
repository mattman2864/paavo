#include <raylib.h>
#include "penguin.h"

void drawPenguin(Penguin* Penguin) {
  DrawRectangle(
      Penguin->x, Penguin->y,
      Penguin->width, Penguin->height,
      RAYWHITE);
}

void movePenguin(Penguin *penguin, Vector2 d) {
  penguin->x += d.x;
  penguin->y += d.y;
}
