#pragma once
#include "Sprite.h"
#include "Body.h"
#include "Input.h"

class Player {
private:
  Input input;
  Body body;
  Sprite sprite;
  
public:
  Player(const Player&) = delete;
  Player& operator=(const Player&) = delete;
  
  Player();
  void update(float dt);
  void draw();
  
  Body* getBody();
};

class Object {
private:
  Body body;
};
