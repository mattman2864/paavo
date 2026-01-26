#include "player.h"

#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

#define SPEED 300

Player *player_create() {
  Player *p = malloc(sizeof (Player));

  p->body = (Body){.pos = {0, 0}, .size = {64, 64}, .vel = {0, 0}};
  p->graphics = (Graphics){.texture = LoadTexture("assets/entities/star/star.png")};
  p->health = (Health){.hp = 100};

  return p;
}

void player_draw(Player *player) {
  float scale = player->body.size.x / player->graphics.texture.width; 
  DrawTextureEx(player->graphics.texture, player->body.pos, 0, scale, RAYWHITE);
}

void player_move(Player *player, Vector2 dir) {
  dir = Vector2Normalize(dir);

  player->body.vel = Vector2Scale(dir, SPEED);
}

