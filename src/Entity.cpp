#include "Entity.h"
#include "Control.h"
#include <raylib.h>

Entity::Entity(Control* control, Body* body, Sprite* sprite) :
  control_(control),
  sprite_(sprite),
  body_(body)
{
  sprite->showHitbox = false;
}

void Entity::update(float dt) {
  control_->update(this);
  body_->update(dt);
  sprite_->update(dt);
}

void Entity::draw() {
  sprite_->draw(this);
}

Body* Entity::getBody() {
  return body_;
}
