//
// Created by dm on 4/17/20.
//

#include <support/result.hpp>
#include "systems/PhysicalSystem.hpp"

#include "support/random.hpp"

const SystemID PhysicalSystem::type_id;

PhysicalSystem::PhysicalSystem()
  : static_objects_(defaults::WORLD_BOUNDARY)
  , dynamic_objects_(defaults::WORLD_BOUNDARY) {}

Result<EntityID> PhysicalSystem::CorrectHitbox(PhysicalComponent &obj, double dt) const {
  auto all_intersects = static_objects_.findAllntersectionsWithBox(obj.hitbox_);
  double min_time = 1e10;
  EntityID min_ent = NO_ENTITY;

  for (const auto &wall: all_intersects) {
    auto ph_comp =
      bro::GetComponentManager()->
        GetComponent<PhysicalComponent>(wall);

    CHECK_ERROR(ph_comp);

    const double cur_time = TrackCollision(ph_comp.ValueUnsafe(), obj, dt);

    if (cur_time < min_time) {
      min_time = cur_time;
      min_ent = wall;
    }
  }

  if (min_ent == NO_ENTITY)
    return make_result::Fail(NOT_FOUND);

  RevertHitbox(&obj, dt - min_time);

  return make_result::Ok(min_ent);
}

void PhysicalSystem::Accept(EventPtr event_ptr) {}

void PhysicalSystem::Update(Duration delta_time) {
  double dt = DurationToDouble(delta_time);

  auto* comp_man = bro::GetComponentManager();

  for (auto component_it =
       comp_man->GetIteratorBegin<PhysicalComponent>();
       component_it != comp_man->GetIteratorEnd<PhysicalComponent>();
       ++component_it) {

    dynamic_objects_.erase(component_it->hitbox_, component_it->entity_id);
    UpdateHitbox(component_it, dt);
    dynamic_objects_.insert(component_it->hitbox_, component_it->entity_id);

    auto collided = CorrectHitbox(*component_it, dt);
    if (collided.IsOk()) {
      std::cout << "wall hit" << std::endl;
      // bro::GetEventManager()->sendMessage<CollisionMessage>(moving_object, colided);
    }
    /*EventManager.sendMessage<MoveMessage>(moving_object, component.hitbox_); */
  }

  for (const auto &pair : dynamic_objects_.findAllIntersections()) {
    REPORT_IF_ERROR(bro::RegisterEvent<CollisionEvent>(pair.first, pair.second));

    std::cout << "collided" << std::endl;
  }
}

void PhysicalSystem::UpdateHitbox(PhysicalComponent* obj, const double dt) {
//  obj->force_ += rnd::GetVector2fNormalRand() * 0.5f;
  obj->velocity_ += obj->force_ / obj->mass_;
  const auto offset = obj->velocity_ * static_cast<float>(dt);
  obj->hitbox_ = obj->hitbox_ + offset;

  REPORT_IF_ERROR(bro::RegisterEvent<MoveEvent>(obj->entity_id, offset));
}

void PhysicalSystem::RevertHitbox(PhysicalComponent* obj, const double dt) {
  const auto offset = obj->velocity_ * static_cast<float>(dt);
  obj->hitbox_ = obj->hitbox_ + obj->velocity_ * static_cast<float>(dt);

  REPORT_IF_ERROR(bro::RegisterEvent<MoveEvent>(obj->entity_id, offset));
}

double PhysicalSystem::TrackCollision(
    PhysicalComponent* this_obj,
    const PhysicalComponent& other_obj,
    const double dt) {
  double rt = dt;
  double lt = 0;
  while (rt - lt > PhysicalComponent::EPS) {
    const double mt = (rt + lt) / 2;
    if (sf::FloatRect(other_obj.hitbox_ +
        (other_obj.velocity_ * static_cast<float>(mt))).intersects(this_obj->hitbox_)) {
      rt = mt;
    } else {
      lt = mt;
    }
  }
  return lt;
}
