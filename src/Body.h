#pragma once
#include "raylib.h"

class Body {
private:
  Vector2 pos = {0, 0};
  Vector2 vel = {0, 0};
  Rectangle hitbox;
  
public:
  void setVelocity(Vector2 velocity);
  void update(float dt);
  Vector2 getPosition() const;
  Rectangle getHitbox() const;
};
