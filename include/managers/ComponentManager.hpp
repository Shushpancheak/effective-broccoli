#ifndef EFFECTIVE_BROCOLLI_COMPONENTMANAGER_HPP
#define EFFECTIVE_BROCOLLI_COMPONENTMANAGER_HPP

#include <unordered_map>
#include <utility>
#include "components/Component.hpp"
#include "constants/error.hpp"
#include "memory/ObjectPool.hpp"
#include "support/typedefs.hpp"
#include "support/result.hpp"

class ComponentManager {
  using ComponentPtr = Component*;

public:
  /**
   * @return component pointer if component exists, nullptr otherwise.
   */
  template<typename T>
  Result<T*> GetComponent(EntityID entity_id);

  /**
   * @return ALLOC_FAILED if object pool is full.
   */
  template<typename T, typename ...Args>
  Status AddComponent(EntityID entity_id, Args&&... args);

  /**
  * @return NOT_FOUND if value does not exist.
  */
  template<typename T>
  Status DeleteComponent(EntityID entity_id);

  Status DeleteAllComponents(EntityID entity_id);
private:
  ObjectPool<COMPONENT_MAX> component_pool_;
  std::unordered_map<EntityID, std::unordered_map<ComponentID, ComponentPtr>> map_;
};

template<typename T, typename ...Args>
Status ComponentManager::AddComponent(const EntityID entity_id, Args&&... args) {
  auto ptr = component_pool_.CreateObject<T>(entity_id, std::forward<Args>(args)...);
  if (ptr.HasError()) {
    return make_result::Fail(ALLOC_FAILED);
  }
  map_[entity_id][T::type_id] = ptr.ValueUnsafe();
  return make_result::Ok();
}

template<typename T>
Status ComponentManager::DeleteComponent(const EntityID entity_id) {
  if (map_[entity_id].count(T::type_id) == 0) {
    return make_result::Fail(NOT_FOUND);
  }
  auto res = component_pool_.DeleteObject<T>(map_[entity_id][T::type_id] );
  if (res.HasError()) {
    return res;
  }
  map_[entity_id].erase(T::type_id);
  return make_result::Ok();
}

template<typename T>
Result<T*> ComponentManager::GetComponent(const EntityID entity_id) {
  if (map_[entity_id].count(T::type_id) == 0) {
    return make_result::Fail(NOT_FOUND);
  }
  return make_result::Ok(static_cast<T*>(map_[entity_id][T::type_id]));
}

#endif //EFFECTIVE_BROCOLLI_COMPONENTMANAGER_HPP
