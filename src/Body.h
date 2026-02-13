#pragma once
#include "raylib.h"

class Body {
private:
  Vector2 pos_;
  Vector2 vel_ = { 0 };
  Rectangle hitbox_;
  
public:
  Body(Vector2 pos, Rectangle hitbox);
  Body();
  void setVelocity(Vector2 velocity);
  void update(float dt);
  Vector2 getPosition() const;
  Rectangle getHitbox() const;
};
