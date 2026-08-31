#include "animation.h"
#include <math.h>
#include <raylib.h>

void animation_update(Animation *animation) {
  animation->frame = (int)round(GetTime() / (1.0 / animation->framerate)) % animation->num_frames;
}

void animation_draw(Animation *animation, Vector2 position) {
  Rectangle source = {.x = animation->size.x * animation->frame, .y = 0, .width = animation->size.x, .height = animation->size.y};
  DrawTexturePro(animation->source, source, (Rectangle){
      .x = position.x, .y = position.y,
      .width = animation->size.x * animation->scale, .height = animation->size.y * animation->scale
      },
      (Vector2){0, 0}, 0, WHITE);
}
