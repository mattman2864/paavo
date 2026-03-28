#include "EntityFactory.h"
#include "Control.h"
#include "Entity.h"
#include "Sprite.h"

Entity EntityFactory::createEntity(int id) {
  EntityData data = entityDatabase[id];
  Control control;
  Body body;
  Sprite *sprite = new Sprite(data.texture.c_str());
  if (id == 0) {
    control = new KeyboardControl(DEFAULT_KB);
  }
  return new Entity(

      ) 
}
