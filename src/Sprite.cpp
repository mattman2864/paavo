#include "Sprite.h"
#include "Entity.h"
#include <raylib.h>

Sprite::Sprite(const char* filename) {
  texture = LoadTexture(filename);
}

void Sprite::draw(Entity* p) {
  float scale = 1;
  Body *b = p->getBody();
  Vector2 pos = p->getBody()->getPosition();
  Rectangle hb = p->getBody()->getHitbox();
  Vector2 drawPosition = {
    .x = pos.x - hb.width/2,
    .y = pos.y - hb.height/2,
  };
  DrawTextureEx(texture, drawPosition, 0, scale, WHITE);
  if (this->showHitbox) {
    DrawRectangleLinesEx(hb, 3, RED);
  }
}

void Sprite::update(float dt) {
  // Implementation
}
