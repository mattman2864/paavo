#include "Entity.h"
#include <vector>

class EntityFactory {
  static std::vector<EntityData> entityDatabase;
  static Entity createEntity(int id); 
};
