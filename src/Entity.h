#pragma once
#include "Sprite.h"
#include "Body.h"
#include "Control.h"

class Entity {
private:
  Control* control_;
  Body* body_;
  Sprite* sprite_;

public:
  Entity(const Entity&) = delete;
  Entity& operator=(const Entity&) = delete;
  
  Entity(Control* control, Body* body, Sprite* sprite);
  void update(float dt);
  void draw();
  
  Body* getBody();
};
