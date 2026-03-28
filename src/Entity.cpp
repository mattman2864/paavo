#include "Entity.h"
#include "Control.h"
#include <raylib.h>
#include <string>

Entity::Entity(Control* control, Body* body, Sprite* sprite, std::string name) :
  control_(control),
  sprite_(sprite),
  body_(body),
  name(name)
{}

void Entity::update(float dt) {
  control_->update(this);
  body_->update(dt);
  sprite_->update(dt);
}

void Entity::draw(bool debug) {
  sprite_->draw(this, debug);
}

Body* Entity::getBody() {
  return body_;
}

std::string Entity::getName() {
  return name; 
}
