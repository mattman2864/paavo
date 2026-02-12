#pragma once
#include "raylib.h"

class Player;  // Forward declaration

class Sprite {
private:
  Texture2D texture;
  
public:
  bool showHitbox = false;

  Sprite(const char* filename);
  void draw(Player* p, Camera2D* c);
  void update(float dt);
};
