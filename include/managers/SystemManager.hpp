#ifndef EFFECTIVE_BROCOLLI_SYSTEMMANAGER_HPP
#define EFFECTIVE_BROCOLLI_SYSTEMMANAGER_HPP


#include <unordered_map>
#include <utility>

#include "systems/System.hpp"

#include "constants/error.hpp"
#include "memory/ObjectPool.hpp"
#include "support/typedefs.hpp"
#include "support/result.hpp"

using SystemPtr = System*;

class SystemManager {
public:
  SystemManager() = default;

  /**
   * Get pointer to a system using its id.
   */
  Result<SystemPtr> GetSystem(SystemID sys_id);

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
  ObjectPool<SYSTEM_MAX> system_pool_;
  std::unordered_map<SystemID, SystemPtr> map_;
};

template<typename T, typename... Args>
Status SystemManager::AddSystem(Args&&... args) {
  T* ptr = system_pool_.CreateObject<T>(std::forward<Args>(args)...);

  if (!ptr) {
    return make_result::Fail(ALLOC_FAILED);
  }

  map_[T::type_id] = ptr;
  return make_result::Ok();
}

template<typename T>
Status SystemManager::DeleteSystem() {
  if (map_.count(T::type_id) == 0) {
    return make_result::Fail(NOT_FOUND);
  }

  auto res = system_pool_.DeleteObject<T>(map_[T::type_id]);
  if (res.HasError()) {
    return res;
  }

  map_[T::type_id].erase(T::type_id);
  return make_result::Ok();
}

template<typename T>
Result<T*> SystemManager::GetSystem() {
  if (map_.count(T::type_id) == 0) {
    return make_result::Fail(NOT_FOUND);
  }

  return make_result::Ok(dynamic_cast<T*>(map_[T::type_id]));
}

#endif //EFFECTIVE_BROCOLLI_SYSTEMMANAGER_HPP
