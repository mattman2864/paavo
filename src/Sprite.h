#pragma once
#include "raylib.h"

class Entity;

class Sprite {
private:
  Texture2D texture;
  
public:
  bool showHitbox = false;

  Sprite(const char* filename);
  void draw(Entity* p);
  void update(float dt);
};
