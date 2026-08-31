#include <raylib.h>
#include <raymath.h>
#include "entity.h"
#include "animation.h"


void update_entity(Entity *entity) {
  animation_update(entity->animation);
}

void draw_entity(Entity* entity) {
  animation_draw(entity->animation, (Vector2){entity->pos.x, entity->pos.y});
}

void move_entity(Entity *entity, Vector2 d, float dt) {
  d = Vector2Normalize(d);
  d = Vector2Scale(d, 300.f);
  entity->pos.x += d.x * dt;
  entity->pos.y += d.y * dt;
}
