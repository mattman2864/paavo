#pragma once

class Entity;

typedef struct {
  int up, down, left, right;
} Keybinds;

#define DEFAULT_KB (Keybinds) {\
    .up = KEY_E,\
    .down = KEY_D,\
    .left = KEY_S,\
    .right = KEY_F\
  }

class Control {
public:
  virtual void update(Entity* entity) = 0;
  virtual ~Control() = default;
};

class KeyboardControl : public Control {
private:
  Keybinds keybinds;
public:
  KeyboardControl(Keybinds keybinds);
  void update(Entity* entity) override;
};

class WalkRightControl: public Control  {
  public:

    void update(Entity* entity) override;
};
