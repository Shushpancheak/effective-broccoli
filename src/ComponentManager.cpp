#include "ComponentManager.hpp"


int ComponentManager::DeleteAllComponents(EntityID entity_id) {
  for (auto& it : map_[entity_id]) {
    component_pool_.Free(it.second);
  }
  map_.erase(entity_id);
  return NO_ERROR;
}