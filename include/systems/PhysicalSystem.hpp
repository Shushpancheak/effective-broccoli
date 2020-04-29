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

  Result<EntityID> CorrectHitbox(PhysicalComponent &obj, double dt) const;

  QuadTree<EntityID> static_objects_;
  QuadTree<EntityID> dynamic_objects_;
  std::unordered_set<EntityID> moving_objects_;
};

#endif //EFFECTIVE_BROCOLLI__PHYSICALSYSTEM_HPP_
