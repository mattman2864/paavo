#pragma once

#include "body.h"
#include "health.h"
#include "graphics.h"

typedef struct {
  Body body;
  Health health;
  Graphics graphics;
} Player;

Player *player_create();
void player_draw(Player *player);
void player_move(Player *player, Vector2 dir);
