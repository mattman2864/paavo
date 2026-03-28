#include "World.h"

World::World() {
  return;
}

void World::addEntity(Entity *e) {
  this->entities.push_back(e);
}

void World::draw(bool debug) {
  for (Entity *e : this->entities) {
    e->draw(debug);
  }
}

void World::update(float dt) {
  for (Entity *e : this->entities) {
    e->update(dt);
  }
}
