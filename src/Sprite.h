#pragma once
#include "raylib.h"

class Entity;

class Sprite {
private:
  Texture2D texture;
  
public:
  Sprite(const char* filename);
  void draw(Entity* p, bool debug = false);
  void update(float dt);
};
