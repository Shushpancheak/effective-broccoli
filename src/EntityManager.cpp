#include "EntityManager.hpp"


Result<EntityPtr> EntityManager::GetEntity(const size_t entity_id) {
  if (map_.count(entity_id) == 0) {
    return make_result::Fail(NOT_FOUND);
  }

  return make_result::Ok(map_[entity_id]);
}

Status EntityManager::DeletePodEntity(const size_t entity_id) {
  if (map_.count(entity_id) == 0) {
    return make_result::Fail(NOT_FOUND);
  }

  const auto target = map_[entity_id];
  auto res = entity_pool_.Free(target);
  CHECK_ERROR(res);
  map_.erase(entity_id);

  return make_result::Ok();
}