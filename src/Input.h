#pragma once

class Player;

typedef struct {
  int up, down, left, right;
} Keybinds;

#define DEFAULT_KB (Keybinds) {\
    .up = KEY_E,\
    .down = KEY_D,\
    .left = KEY_S,\
    .right = KEY_F\
  }

class Input {
private:
  Keybinds keybinds;
  float walkingSpeed = 300.0f;
  
public:
  Input(Keybinds keybinds);
  void update(Player* player);
};
