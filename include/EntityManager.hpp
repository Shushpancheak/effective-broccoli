//
// Created by alex on 3/24/20.
//

#ifndef EFFECTIVE_BROCOLLI_ENTITY_MANAGER_HPP
#define EFFECTIVE_BROCOLLI_ENTITY_MANAGER_HPP

#include <unordered_map>
#include "Entity.hpp"
#include "memory/ObjectPool.hpp"
#include "support/result.hpp"

using EntityPtr = void*;

class EntityManager {
public:
  Result<EntityPtr> GetEntity(size_t entity_id);
  Status            DeletePodEntity(size_t entity_id);
  template<typename T>
  Status            DeleteEntity(size_t entity_id);

  template<typename T, typename... Args>
  Result<size_t>    AddEntity(Args&&... args);

private:
  static size_t current_id_;
  std::unordered_map<size_t, EntityPtr> map_;
  ObjectPool entity_pool_;
};

size_t EntityManager::current_id_ = 0;

template<typename T, typename... Args>
Result<size_t> EntityManager::AddEntity(Args&&... args) {
  auto entity_id = EntityManager::current_id_++;
  T* ptr = entity_pool_.CreateObject<T>(entity_id, std::forward<Args>(args)...);
  if (!ptr) {
    return make_result::Fail(ALLOC_FAILED);
  }
  map_[entity_id] = ptr;
  return make_result::Ok(entity_id);
}

template<typename T>
Status EntityManager::DeleteEntity(const size_t entity_id) {
  if (map_.count(entity_id) == 0) {
    return make_result::Fail(NOT_FOUND);
  }

  const auto target = map_[entity_id];
  auto res = entity_pool_.DeleteObject<T>(target);
  CHECK_ERROR(res);
  map_.erase(entity_id);

  return make_result::Ok();
}


#endif //EFFECTIVE_BROCOLLI_ENTITY_MANAGER_HPP
