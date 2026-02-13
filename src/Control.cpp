#include "Control.h"
#include "Entity.h"
#include "raylib.h"
#include "raymath.h"
#include <cmath>

KeyboardControl::KeyboardControl(Keybinds keybinds) : keybinds(keybinds) {}

void KeyboardControl::update(Entity* entity) {
  Vector2 dir = {
    (float)(IsKeyDown(keybinds.right) - IsKeyDown(keybinds.left)),
    (float)(IsKeyDown(keybinds.down) - IsKeyDown(keybinds.up))
  };
  
  if (dir.x != 0 || dir.y != 0) {
    dir = Vector2Normalize(dir);
  }
  
  Vector2 vel = Vector2Scale(dir, 300.f);
  entity->getBody()->setVelocity(vel);
}

void WalkRightControl::update(Entity* entity) {
  entity->getBody()->setVelocity({(float)cos(GetTime()) * 200.f, -(float)sin(GetTime()) * 200.f});
};
