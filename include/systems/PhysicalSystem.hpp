//
// Created by dm on 3/23/20.
//

#ifndef EFFECTIVE_BROCOLLI__PHYSICALSYSTEM_HPP_
#define EFFECTIVE_BROCOLLI__PHYSICALSYSTEM_HPP_
#include <unordered_set>
#include "entities/Entity.hpp"
#include "components/PhysicalComponent.hpp"
#include "support/QuadTree.hpp"
#include "System.hpp"

#include "engine/core.hpp"

#include "events/transform_events.hpp"
#include "events/physical_events.hpp"

// Group - bitmask? or register groups
class PhysicalSystem : public System {
  static const EntityID NO_ENTITY = static_cast<EntityID>(-1);

 public:
  static const SystemID type_id = SYSTEM_PHYSICAL;

  PhysicalSystem();

  // int AddEntity(PhysicalComponent *component, int group);
  // int DeleteEntity(PhysicalComponent *component, int group);

  virtual void Accept(EventPtr event_ptr) override;

 private:
  virtual void Update(Duration delta_time) override;

  /**
   * Update hitbox by implicit Euler integration
   * @param obj the pointer object's physical component to update
   * @param dt delta time
   */
  static void UpdateHitbox(PhysicalComponent* obj, double dt);

  /**
   * Revert hitbox by time dt.
   * @param obj the pointer object's physical component to update
   * @param dt delta time
   */
  static void RevertHitbox(PhysicalComponent* obj, double dt);

  /**
   * Returns time that is needed for first collision of other_obj to this one.
   */
  static double TrackCollision(PhysicalComponent* this_obj,
                               const PhysicalComponent& other_obj,
                               double dt);

  Result<EntityID> CorrectHitbox(PhysicalComponent& obj, double dt) const;

  QuadTree<EntityID> static_objects_;
  QuadTree<EntityID> dynamic_objects_;
};

#endif //EFFECTIVE_BROCOLLI__PHYSICALSYSTEM_HPP_
