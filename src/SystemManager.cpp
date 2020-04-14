#include "managers/SystemManager.hpp"

Result<SystemPtr> SystemManager::GetSystem(const SystemID sys_id) {
  if (map_.count(sys_id) == 0) {
    return make_result::Fail(NOT_FOUND);
  }

  return make_result::Ok(map_[sys_id]);
}
