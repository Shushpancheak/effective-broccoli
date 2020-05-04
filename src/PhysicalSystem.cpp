//
// Created by dm on 4/17/20.
//

#include <support/result.hpp>
#include "systems/PhysicalSystem.hpp"

Result<EntityID> PhysicalSystem::CorrectHitbox(PhysicalComponent &obj, double dt) const {
  auto all_intersects = static_objects_.findAllntersectionsWithBox(obj.hitbox_);
  double min_time = 1e10;
  EntityID min_ent = NO_ENTITY;

  for (const auto &wall: all_intersects) {
    auto ph_comp =
      bro::GetComponentManager()->
        GetComponent<PhysicalComponent>(wall);

    CHECK_ERROR(ph_comp);

    const double cur_time = ph_comp.ValueUnsafe()->TrackCollision(obj, dt);

    if (cur_time < min_time) {
      min_time = cur_time;
      min_ent = wall;
    }
  }

  obj.RevertHitbox(dt - min_time);
  if (min_ent != NO_ENTITY)
    return make_result::Ok(min_ent);

  return make_result::Fail(NOT_FOUND);
}

PhysicalSystem::PhysicalSystem()
  : static_objects_(defaults::WORLD_BOUNDARY)
  , dynamic_objects_(defaults::WORLD_BOUNDARY)
  , moving_objects_() {}

void PhysicalSystem::AddMovingEntity(const EntityID entity_id) {
  moving_objects_.insert(entity_id);
}

void PhysicalSystem::Accept(EventPtr event_ptr) {}

void PhysicalSystem::Update(Duration delta_time) {
  double dt = DurationToDouble(delta_time);

  for (const auto moving_obj: moving_objects_) {
    auto component_res =
      bro::GetComponentManager()->
        GetComponent<PhysicalComponent>(moving_obj);
    REPORT_IF_ERROR(component_res);

    auto* component = component_res.ValueUnsafe();

    dynamic_objects_.erase(component->hitbox_, moving_obj);
    component->UpdateHitbox(dt);
    dynamic_objects_.insert(component->hitbox_, moving_obj);

    auto collided = CorrectHitbox(*component, dt);
    if (collided.IsOk()) {
      std::cout << "wall hit" << std::endl;
      // bro::GetEventManager()->sendMessage<CollisionMessage>(moving_object, colided);
    }
  }

  for (const auto &pair : dynamic_objects_.findAllIntersections()) {
    // EventManager.sendMessage<CollisionMessage>(pair);
  }
}
