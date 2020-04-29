//
// Created by dm on 3/23/20.
//
#include "components/PhysicalComponent.hpp"

PhysicalComponent::PhysicalComponent(sf::Rect<float> hitbox_, float mass_, PhysicalGroup group):
 hitbox_(hitbox_),
 mass_(mass_),
 velocity_(0, 0),
 group_(group) {}


sf::FloatRect PhysicalComponent::GetHitbox() {
  return hitbox_;
}

double PhysicalComponent::trackCollision(const PhysicalComponent &object, Duration dt) const {
  double rt = DurationToDouble(dt);
  double lt = 0;
  while (rt - lt > EPS) {
    double mt = (rt + lt) / 2;
    if (sf::FloatRect(object.hitbox_ + (object.velocity_ * (float)mt)).intersects(hitbox_)) {
      rt = mt;
    } else {
      lt = mt;
    }
  }
  return lt;
}

void PhysicalComponent::UpdateHitbox(double dt) {
  velocity_ += force_ / mass_;
  hitbox_ =  hitbox_ + velocity_ * (float)dt;
}

void PhysicalComponent::revertHitbox(double dt) {
  hitbox_ = hitbox_ + velocity_ *(float)-dt;
}

