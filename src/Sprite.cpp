#include "Sprite.h"
#include "Entity.h"
#include <raylib.h>
#include <raymath.h>

Sprite::Sprite(const char* filename) {
  texture = LoadTexture(filename);
}

void Sprite::draw(Entity* e, bool debug) {
  float scale = 1;
  Body *b = e->getBody();
  Vector2 pos = e->getBody()->getPosition();
  Rectangle hb = e->getBody()->getHitbox();
  Vector2 drawPosition = {
    .x = pos.x - hb.width/2,
    .y = pos.y - hb.height/2,
  };
  DrawTextureEx(texture, drawPosition, 0, scale, WHITE);
  if (debug) {
    DrawText(e->getName().c_str(), hb.x, hb.y + hb.height, 12, BLACK);
    DrawRectangleLinesEx(hb, 1, RED);
  }
}

void Sprite::update(float dt) {
  // Implementation
}
