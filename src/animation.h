#pragma once

#include <raylib.h>

typedef struct {
  int frame_count;
  int frame_rate;
  Vector2 size;
  Texture2D source;
} AnimationData;

typedef struct {
  int frame;
  float scale;
  AnimationData *data;
} Animation;

void animation_update(Animation *animation);
void animation_draw(Animation *animation, Vector2 position);
