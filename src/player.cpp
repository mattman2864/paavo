#include <raylib.h>
#include <raymath.h>
#include <cmath>
#include <string>

static constexpr float RAD = 10.f;

class Player {
private:
  Vector2 pos;
  Vector2 dashVec;
  float speed = 300.f;
  float dashLength = 300.f;
  Texture2D texture;

  std::string debugText = "";
public:
  Player(float x, float y) 
    : pos{ x, y } {
    texture = LoadTexture("assets/entities/star/star.png");
  }

  void update(float dt) {
    float dashLen = Vector2Length(dashVec);
    if (dashLen > 0) {
      Vector2 delta = Vector2Scale(dashVec, dt * 0.1 * dashLen);
      this->pos = Vector2Add(pos, delta);
      this->dashVec = Vector2Subtract(dashVec, delta);

      if (dashLen < 100) {
        dashVec = {0, 0};
      }
    }
  }

  void draw() const {
    DrawTextureEx(texture, pos, 0, 4.f, WHITE);
    DrawText(&(debugText[0]), pos.x - 2 * debugText.length(), pos.y + 80, 20, BLACK);
  }

  void move(Vector2 dir, float dt) {
    dir = Vector2Normalize(dir);
    this->pos = Vector2Add(pos, Vector2Scale(dir, speed * dt));
  }

  void dash(Vector2 dir) {
    dir = Vector2Normalize(dir);
    this->dashVec = Vector2Scale(dir, dashLength);
  }
};
