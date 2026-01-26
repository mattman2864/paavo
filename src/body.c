#include "body.h"
#include <raymath.h>

void body_update(Body *body, float dt) {
  body->pos = Vector2Add(body->pos, Vector2Scale(body->vel, dt));
}
