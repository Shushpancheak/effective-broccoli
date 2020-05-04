//
// Created by alex on 3/24/20.
//

#ifndef EFFECTIVE_BROCOLLI_ENTITY_MANAGER_HPP
#define EFFECTIVE_BROCOLLI_ENTITY_MANAGER_HPP

#include <unordered_map>
#include "entities/Entity.hpp"
#include "memory/ObjectPool.hpp"
#include "support/result.hpp"
#include "support/typedefs.hpp"

using EntityPtr = Entity*;

class EntityManager {
public:
  Result<EntityPtr> GetEntity(EntityID entity_id);
  Status            DeletePodEntity(EntityID entity_id);
  template<typename T>
  Status            DeleteEntity(EntityID entity_id);

  template<typename T, typename... Args>
  Result<EntityID>  AddEntity(Args&&... args);

private:
  static EntityID current_id_;
  std::unordered_map<EntityID, EntityPtr> map_;
  ObjectPool<ENTITY_MAX> entity_pool_;
};

inline EntityID EntityManager::current_id_ = 0;

template<typename T, typename... Args>
Result<EntityID> EntityManager::AddEntity(Args&&... args) {
  auto entity_id = EntityManager::current_id_++;
  auto ptr = entity_pool_.CreateObject<T>(entity_id, std::forward<Args>(args)...);
  if (ptr.HasError()) {
    return make_result::Fail(ALLOC_FAILED);
  }
  map_[entity_id] = ptr.ValueUnsafe();
  return make_result::Ok(entity_id);
}

template<typename T>
Status EntityManager::DeleteEntity(const EntityID entity_id) {
  if (map_.count(entity_id) == 0) {
    return make_result::Fail(NOT_FOUND);
  }

  auto* const target = map_[entity_id];
  auto res = entity_pool_.DeleteObject<T>(target);
  CHECK_ERROR(res);
  map_.erase(entity_id);

  return make_result::Ok();
}


#endif //EFFECTIVE_BROCOLLI_ENTITY_MANAGER_HPP
