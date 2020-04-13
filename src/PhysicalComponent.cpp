//
// Created by dm on 3/23/20.
//
#include "PhysicalComponent.hpp"
#include "PhysicalSystem.hpp"

PhysicalComponent::PhysicalComponent(sf::Rect<float> hitbox_, float mass_, int group):
 hitbox_(hitbox_),
 mass_(mass_),
 velocity_(0, 0),
 group_(group) {
  PhysicalSystem::GetInstance()->AddEntity(this, group);
}

PhysicalComponent::~PhysicalComponent() {
  PhysicalSystem::GetInstance()->DeleteEntity(this, group_);
}
const sf::FloatRect &PhysicalComponent::GetHitbox() {
  return hitbox_;
}
