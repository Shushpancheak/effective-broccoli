#ifndef EFFECTIVE_BROCOLLI_SYSTEMMANAGER_HPP
#define EFFECTIVE_BROCOLLI_SYSTEMMANAGER_HPP


#include <unordered_map>
#include <utility>
#include "systems/System.hpp"
#include "constants/error.hpp"
#include "memory/ObjectPool.hpp"
#include "support/typedefs.hpp"
#include "support/result.hpp"


class SystemManager {
  using SystemPtr = void*;
public:
  /**
   * @return system pointer if system exists, error otherwise.
   */
  template<typename T>
  Result<T*> GetSystem();

  /**
   * @return ALLOC_FAILED if object pool is full.
   */
  template<typename T, typename ...Args>
  Status AddSystem(Args&&... args);

  /**
  * @return NOT_FOUND if value does not exist.
  */
  template<typename T>
  Status DeleteSystem();

private:
  ObjectPool system_pool_;
  std::unordered_map<SystemID, SystemPtr> map_;
};

template<typename T, typename... Args>
Status SystemManager::AddSystem(Args&&... args) {
  T* ptr = system_pool_.CreateObject<T>(std::forward<Args>(args)...);
  if (!ptr) {
    return make_result::Fail(ALLOC_FAILED);
  }
  map_[T::GetTypeId()] = ptr;
  return make_result::Ok();
}

template<typename T>
Status SystemManager::DeleteSystem() {
  if (map_.count(T::GetTypeID()) == 0) {
    return make_result::Fail(NOT_FOUND);
  }
  auto res = system_pool_.DeleteObject<T>(map_[T::GetTypeId()]);
  if (res.HasError()) {
    return res;
  }
  map_[T::GetTypeId()].erase(T::GetTypeID());
  return make_result::Ok();
}

template<typename T>
Result<T*> SystemManager::GetSystem() {
  if (map_.count(T::GetTypeID()) == 0) {
    return make_result::Fail(NOT_FOUND);
  }
  return make_result::Ok(static_cast<T*>(map_[T::GetTypeID()]));
}

#endif //EFFECTIVE_BROCOLLI_SYSTEMMANAGER_HPP
