#ifndef EFFECTIVE_BROCOLLI_COMPONENTMANAGER_HPP
#define EFFECTIVE_BROCOLLI_COMPONENTMANAGER_HPP

#include <unordered_map>
#include <utility>
#include "Component.hpp"
#include "ObjectPool.hpp"

using entityID_t = size_t;
using componentID_t = size_t;
using ComponentPtr = void*;

enum ComponentManagerErrorCodes {
  NO_ERROR,
  NOT_EXIST,
  ALLOC_FAULT
};

class ComponentManager {
public:
  /**
   * @return component pointer if component exists, nullptr otherwise
   * **/
  template<typename T>
  T* GetComponent(entityID_t entity_id);

  /**
   * @return ALLOC_FAULT if object pool is full
   * **/
  template<typename T, typename ...Args>
  int AddComponent(entityID_t entity_id, Args&&... args);

    /**
   * @return NOT_EXIST if value does not exist
   * **/
  template<typename T>
  int DeleteComponent(entityID_t entity_id);

  int DeleteAllComponents(entityID_t entity_id);
private:
  ObjectPool component_pool_; // T& pool.CreateObject<T>(), pool.Delete()
  std::unordered_map<entityID_t , std::unordered_map<componentID_t, ComponentPtr>> map_;
};

template<typename T, typename ...Args>
int ComponentManager::AddComponent(entityID_t entity_id, Args&&... args) {
  T* ptr = component_pool_.CreateObject<T>(std::forward(args)...);
  if (!ptr) {
    return ALLOC_FAULT;
  }
  map_[entity_id][T::GetTypeID()] = ptr;
  return NO_ERROR;
}

template<typename T>
int ComponentManager::DeleteComponent(entityID_t entity_id) {
  if (map_[entity_id].count(T::GetTypeID()) == 0) {
    return NOT_EXIST;
  }
  component_pool_.Delete(map_[entity_id][T::GetTypeID()] );
  map_[entity_id].erase(T::GetTypeID());
  return NO_ERROR;
}

template<typename T>
T *ComponentManager::GetComponent(entityID_t entity_id) {
  if (map_[entity_id].count(T::GetTypeID()) == 0) {
    return nullptr;
  }
  return map_[entity_id][T::GetTypeID()];
}

#endif //EFFECTIVE_BROCOLLI_COMPONENTMANAGER_HPP
