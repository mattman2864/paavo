#include <raylib.h>

struct Hitbox {
  Vector2 pos{};
  Vector2 size{};
  float rot = 0;

  Rectangle GetRect() const {
    return { pos.x, pos.y, size.x, size.y};
  }
};

struct Movement {
  Vector2 vel{};
  float speed = 200.f;
};

struct Animation {
  Texture2D* spriteSheet = nullptr;
  Vector2 framesize{};
  int currentFrame = 0;
  int frameCount = 0;
  float frameTime = 0.1f;
  float timer = 0.0f;

  void Update(float dt) {
    timer += dt;
    if (timer >= frameTime) {}
  }
};

struct Health {
  int current;
  int max;
  bool isAlive() const { return current > 0; }
};
