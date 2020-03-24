#include "EntityManager.hpp"


EntityPtr EntityManager::GetEntity(size_t entity_id) {
  if (map_.count(entity_id) == 0) {
    return nullptr;
  }
  return map_[entity_id];
}

int EntityManager::DeleteEntity(size_t entity_id) {
  if (map_.count(entity_id) == 0) {
    return NOT_EXIST;
  }
  auto target = map_[entity_id];
  entity_pool_.Delete(target);
  map_.erase(entity_id);
  return NO_ERROR;
}