#include "Entity.h"
#include <vector>

class World {
private: 
  std::vector<Entity *> entities;
public:
  World();
  void addEntity(Entity *);
  void draw(bool debug = false);
  void update(float);
};
