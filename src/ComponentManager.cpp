#include "managers/ComponentManager.hpp"


Status ComponentManager::DeleteAllComponents(EntityID entity_id) {
  for (auto& it : map_[entity_id]) {
    auto res = component_pool_.Free(it.second);
    if (res.HasError()) {
      return res;
    }
  }
  map_.erase(entity_id);
  return make_result::Ok();
}