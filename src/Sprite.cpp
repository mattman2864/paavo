#include "Sprite.h"
#include "Player.h"
#include <raylib.h>

Sprite::Sprite(const char* filename) {
  texture = LoadTexture(filename);
}

void Sprite::draw(Player* p, Camera2D* c) {
  float scale = 1;
  Body *b = p->getBody();
  DrawTextureEx(texture, b->getPosition(), 0, scale, WHITE);
  if (this->showHitbox) {
    Rectangle hitbox = {
      .x = b->getPosition().x,
      .y = b->getPosition().y,
      .width = b->getHitbox().width,
      .height = b->getHitbox().height
    };
    DrawRectangleLinesEx(hitbox, 3, RED);
  }
}

void Sprite::update(float dt) {
  // Implementation
}
