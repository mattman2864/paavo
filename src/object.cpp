#pragma once

#include <raylib.h>
#include <string>
class Object {
private:
  Texture2D texture;
  Rectangle hitbox;
public:
  Object(std::string filename, Vector2 pos, Vector2 size) {
    texture = LoadTexture(filename.c_str());
    hitbox = (Rectangle){pos.x, pos.y, size.x, size.y};
  }

  void draw() {
    DrawTextureEx(texture, {hitbox.x, hitbox.y}, 0, 4.f, WHITE);
  }

  void unload() {
    UnloadTexture(texture);
  }
};
