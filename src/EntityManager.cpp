#include "EntityManager.hpp"


EntityPtr EntityManager::GetEntity(const size_t entity_id) {
  if (map_.count(entity_id) == 0) {
    return nullptr;
  }

  return map_[entity_id];
}

int EntityManager::DeletePodEntity(const size_t entity_id) {
  if (map_.count(entity_id) == 0) {
    return NOT_FOUND;
  }

  const auto target = map_[entity_id];
  entity_pool_.Free(target);
  map_.erase(entity_id);

  return NO_ERROR;
}

template<typename T>
int EntityManager::DeleteEntity(const size_t entity_id) {
  if (map_.count(entity_id) == 0) {
    return NOT_FOUND;
  }

  const auto target = map_[entity_id];
  entity_pool_.DeleteObject<T>(target);
  map_.erase(entity_id);

  return NO_ERROR;
}