#ifndef EFFECTIVE_BROCOLLI_COMPONENTMANAGER_HPP
#define EFFECTIVE_BROCOLLI_COMPONENTMANAGER_HPP

#include <unordered_map>
#include <utility>
#include "Component.hpp"
#include "ObjectPool.hpp"
#include "constants/error_codes.hpp"
#include "support/typedefs.hpp"

class ComponentManager {
  using ComponentPtr = void*;

public:
  /**
   * @return component pointer if component exists, nullptr otherwise.
   */
  template<typename T>
  T* GetComponent(EntityID entity_id);

  /**
   * @return ALLOC_FAILED if object pool is full.
   */
  template<typename T, typename ...Args>
  int AddComponent(EntityID entity_id, Args&&... args);

  /**
  * @return NOT_FOUND if value does not exist.
  */
  template<typename T>
  int DeleteComponent(EntityID entity_id);

  int DeleteAllComponents(EntityID entity_id);
private:
  ObjectPool<> component_pool_;
  std::unordered_map<EntityID, std::unordered_map<ComponentID, ComponentPtr>> map_;
};

template<typename T, typename ...Args>
int ComponentManager::AddComponent(const EntityID entity_id, Args&&... args) {
  T* ptr = component_pool_.CreateObject<T>(std::forward(args)...);
  if (!ptr) {
    return ALLOC_FAILED;
  }
  map_[entity_id][T::GetTypeID()] = ptr;
  return NO_ERROR;
}

template<typename T>
int ComponentManager::DeleteComponent(EntityID entity_id) {
  if (map_[entity_id].count(T::GetTypeID()) == 0) {
    return NOT_FOUND;
  }
  component_pool_.DeleteObject<T>(map_[entity_id][T::GetTypeID()] );
  map_[entity_id].erase(T::GetTypeID());
  return NO_ERROR;
}

template<typename T>
T *ComponentManager::GetComponent(EntityID entity_id) {
  if (map_[entity_id].count(T::GetTypeID()) == 0) {
    return nullptr;
  }
  return map_[entity_id][T::GetTypeID()];
}

#endif //EFFECTIVE_BROCOLLI_COMPONENTMANAGER_HPP
