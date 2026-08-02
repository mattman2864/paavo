#include <raylib.h>

typedef struct {
  float x;
  float y;
  float width;
  float height;
} Penguin;

void drawPenguin(Penguin* penguin);
void movePenguin(Penguin* penguin, Vector2 d, float dt);
