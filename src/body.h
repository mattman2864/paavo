#include <raylib.h>
typedef struct {
  Vector2 pos;
  Vector2 size;
  Vector2 vel;
} Body;

void body_update(Body *body, float dt);
