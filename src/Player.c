#pragma once

#include "RectangleList.c"
#include "raylib.h"

typedef struct {
  Vector2 pos;
  float walkingSpeed;
  int size;
} Player;

Rectangle Player_getHitbox(Player *player) {
  return (Rectangle){player->pos.x, player->pos.y, player->size, player->size};
}

void Player_move(Player *player, Vector2 dir, float dt, RectangleList *colliders) {
  float l = Vector2Length(dir);
  if (l == 0) return; // Avoid divide by zero - No movement
  dir.x /= l;
  dir.y /= l;
  player->pos.x += dir.x * player->walkingSpeed * dt;
  player->pos.y += dir.y * player->walkingSpeed * dt;
  for (int i = 0; i < colliders->count; i++) {
    Rectangle collider = colliders->items[i];
    if (CheckCollisionRecs(Player_getHitbox(player), collider)) {
      Rectangle collision = GetCollisionRec(Player_getHitbox(player), collider);
      if (collision.width > collision.height) {
        if (player->pos.y > collider.y) player->pos.y = collider.y + collider.height;
        else player->pos.y = collider.y - player->size;
      } else {
        if (player->pos.x > collider.x) player->pos.x = collider.x + collider.width;
        else player->pos.x = collider.x - player->size;
      }
    }
  }
}

void Player_draw(Player *player) {
  DrawRectangle(player->pos.x, player->pos.y, player->size, player->size, DARKGRAY);
}
