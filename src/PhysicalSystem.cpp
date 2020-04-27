//
// Created by dm on 4/17/20.
//

#include <support/result.hpp>
#include "systems/PhysicalSystem.hpp"

Result<EntityID> PhysicalSystem::correctHitbox(PhysicalComponent &obj, double dt) {
  auto all_intersects = static_objects_.findAllntersectionsWithBox(obj.hitbox_);
  double min_time = 1e10;
  EntityID min_ent = -1;
  for (const auto &wall: all_intersects) {
    double cur_time = /*ComponentManager.getComponent<PhysicalComponent>(wall).trackCollision(obj, dt)*/ 0;
    if (cur_time < min_time) {
      min_time = cur_time;
      min_ent = wall;
    }
  }
  obj.revertHitbox(dt - min_time);
  if (min_ent != -1)
    return make_result::Ok(min_ent);
  return make_result::Fail();
}

void PhysicalSystem::MoveAllObjects(TimeStamp lastCall) {
  double dt = DurationToDouble(Clock::now() - lastCall);
  for (const auto moving_obj: moving_objects_) {
    auto &component = PhysicalComponent({0, 0, 0, 0}); /*ComponentManager.getComponent<PhysicalComponent>(moving_obj);*/
    dynamic_objects_.erase(component.hitbox_, moving_obj);
    component.UpdateHitbox(dt);
    dynamic_objects_.insert(component.hitbox_, moving_obj);
    auto collided = correctHitbox(component, dt);
    if (collided.IsOk()) {
      /*EventManager.sendMessage<CollisionMessage>(moving_object, colided); */
    }
  }
  for (const auto &pair : dynamic_objects_.findAllIntersections()) {
    /* EventManager.sendMessage<CollisionMessage>(pair); */
  }
}
