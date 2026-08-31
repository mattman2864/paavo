#pragma once

#include <raylib.h>
#include "animation.h"
#include "control.h"

typedef struct {
  // basic data
  Vector2 pos;
  float scale;

  // components
  Movement *movement;
  Graphics *graphics
} Entity;

void update_entity(Entity *entity);
void draw_entity(Entity* entity);
void move_entity(Entity* entity, Vector2 d, float dt);
