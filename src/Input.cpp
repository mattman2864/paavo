#include "Input.h"
#include "Player.h"
#include "raylib.h"
#include "raymath.h"

Input::Input(Keybinds keybinds) : keybinds(keybinds) {}

void Input::update(Player* player) {
  Vector2 dir = {
    (float)(IsKeyDown(keybinds.right) - IsKeyDown(keybinds.left)),
    (float)(IsKeyDown(keybinds.down) - IsKeyDown(keybinds.up))
  };
  
  if (dir.x != 0 || dir.y != 0) {
    dir = Vector2Normalize(dir);
  }
  
  Vector2 vel = Vector2Scale(dir, walkingSpeed);
  player->getBody()->setVelocity(vel);
}
