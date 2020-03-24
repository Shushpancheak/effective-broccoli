#include "ComponentManager.hpp"


int ComponentManager::DeleteAllComponents(entityID_t entity_id) {
  for (auto it = map_[entity_id].begin(); it != map_[entity_id].end(); ++it) {
    component_pool_.Delete(it->second);
  }
  map_.erase(entity_id);
  return NO_ERROR;
}