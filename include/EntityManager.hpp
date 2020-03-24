//
// Created by alex on 3/24/20.
//

#ifndef EFFECTIVE_BROCOLLI_ENTITYMANAGER_HPP
#define EFFECTIVE_BROCOLLI_ENTITYMANAGER_HPP

#include <unordered_map>
#include "Entity.hpp"
#include "ObjectPool.hpp"

using EntityPtr = void*;

enum EntityManagerErrorCodes {
  NO_ERROR,
  NOT_EXIST,
  ALLOC_FAULT
};

class EntityManager {
public:
  EntityPtr GetEntity(size_t entity_id);
  int       DeleteEntity(size_t entity_id);

  template<typename T>
  int       AddEntity();

  static size_t current_id_;
private:
  std::unordered_map<size_t, EntityPtr> map_;
  ObjectPool entity_pool_;
};

size_t EntityManager::current_id_ = 0;

template<typename T>
int EntityManager::AddEntity() {
  auto entity_id = EntityManager::current_id_++;
  T* ptr = entity_pool_.CreateObject<T>(entity_id);
  if (!ptr) {
    return ALLOC_FAULT;
  }
  map_[entity_id] = ptr;
  return NO_ERROR;
}


#endif //EFFECTIVE_BROCOLLI_ENTITYMANAGER_HPP
