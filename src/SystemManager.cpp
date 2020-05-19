#include "managers/SystemManager.hpp"

SystemPtr SystemManager::GetSystem(const SystemID sys_id) {
  assert(map_.count(sys_id) != 0);
  return map_[sys_id];
}
