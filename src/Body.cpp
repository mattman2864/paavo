#include "Body.h"
#include "raymath.h"

Body::Body(Vector2 pos, Rectangle hitbox) :
  pos_(pos), hitbox_(hitbox)
{}

Body::Body() :
  pos_({0, 0}), vel_({0, 0}), hitbox_({.x = -10, .y = -10, .width = 20, .height = 20})
{}

void Body::setVelocity(Vector2 velocity) {
  this->vel_ = velocity;
}

void Body::update(float dt) {
  this->pos_ = Vector2Add(this->pos_, Vector2Scale(this->vel_, dt));
}

Vector2 Body::getPosition() const {
  return pos_;
}

Rectangle Body::getHitbox() const {
  Rectangle globalHB = {
    .x = hitbox_.x + pos_.x,
    .y = hitbox_.y + pos_.y,
    .width = hitbox_.width,
    .height = hitbox_.height
  };
  return globalHB;
}
