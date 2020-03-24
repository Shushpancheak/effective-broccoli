//
// Created by alex on 3/24/20.
//

#ifndef EFFECTIVE_BROCOLLI_ENTITY_MANAGER_HPP
#define EFFECTIVE_BROCOLLI_ENTITY_MANAGER_HPP

#include <unordered_map>
#include "Entity.hpp"
#include "ObjectPool.hpp"

using EntityPtr = void*;

class EntityManager {
public:
  EntityPtr GetEntity(size_t entity_id);
  int       DeletePodEntity(size_t entity_id);
  template<typename T>
  int       DeleteEntity(size_t entity_id);

  template<typename T>
  int       AddEntity();

private:
  static size_t current_id_;
  std::unordered_map<size_t, EntityPtr> map_;
  ObjectPool<> entity_pool_;
};

size_t EntityManager::current_id_ = 0;

template<typename T>
int EntityManager::AddEntity() {
  auto entity_id = EntityManager::current_id_++;
  T* ptr = entity_pool_.CreateObject<T>(entity_id);
  if (!ptr) {
    return ALLOC_FAILED;
  }
  map_[entity_id] = ptr;
  return NO_ERROR;
}


#endif //EFFECTIVE_BROCOLLI_ENTITY_MANAGER_HPP
