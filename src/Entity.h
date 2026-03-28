#pragma once
#include "Sprite.h"
#include "Body.h"
#include "Control.h"
#include <string>

typedef struct {
  int id; 
  std::string name;
  std::string texture;
  Vector2 size;
} EntityData;

class Entity {
private:
  Control* control_;
  Body* body_;
  Sprite* sprite_;
  std::string name;

public:
  Entity(const Entity&) = delete;
  Entity& operator=(const Entity&) = delete;
  
  Entity(Control* control, Body* body, Sprite* sprite, std::string name);
  void update(float dt);
  void draw(bool debug = false);
  
  Body* getBody();
  std::string getName();
};
