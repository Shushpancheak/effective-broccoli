//
// Created by dm on 3/23/20.
//

#ifndef EFFECTIVE_BROCOLLI__PHYSICALSYSTEM_HPP_
#define EFFECTIVE_BROCOLLI__PHYSICALSYSTEM_HPP_
#include "Entity.hpp"
#include "components/PhysicalComponent.hpp"
#include "QuadTree.hpp"
#include "System.hpp"

// Group - bitmask? or register groups
class PhysicalSystem : public System {
 public:
  static const SystemID type_id = SYSTEM_PHYSICAL;

  int AddEntity(PhysicalComponent *component, int group);
  int DeleteEntity(PhysicalComponent *component, int group);
  static PhysicalSystem* GetInstance();
  void Update();

  virtual void Accept(EventPtr event_ptr) override;

 private:
  PhysicalSystem();

  static PhysicalSystem* ph_system_;
  QuadTree static_objects_;
};

#endif //EFFECTIVE_BROCOLLI__PHYSICALSYSTEM_HPP_
