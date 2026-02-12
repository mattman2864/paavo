#include "Body.h"
#include "raymath.h"

void Body::setVelocity(Vector2 velocity) {
  this->vel = velocity;
}

void Body::update(float dt) {
  this->pos = Vector2Add(this->pos, Vector2Scale(this->vel, dt));
}

Vector2 Body::getPosition() const {
  return pos;
}

Rectangle Body::getHitbox() const {
  Rectangle globalHB = {
    .x = hitbox.x + pos.x,
    .y = hitbox.y + pos.y,
    .width = hitbox.width,
    .height = hitbox.height
  };
  return globalHB;
}
