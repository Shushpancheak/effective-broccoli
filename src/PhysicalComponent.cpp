//
// Created by dm on 3/23/20.
//
#include "components/PhysicalComponent.hpp"

PhysicalComponent::PhysicalComponent(const EntityID owner, sf::Rect<float> hitbox_, float mass_, PhysicalGroup group)
  : Component(owner)
  , hitbox_(hitbox_)
  , velocity_(0, 0)
  , mass_(mass_)
  , group_(group) {}


sf::FloatRect PhysicalComponent::GetHitbox() const {
  return hitbox_;
}

double PhysicalComponent::TrackCollision(const PhysicalComponent &object, double dt) const {
  double rt = dt;
  double lt = 0;
  while (rt - lt > EPS) {
    const double mt = (rt + lt) / 2;
    if (sf::FloatRect(object.hitbox_ +
        (object.velocity_ * static_cast<float>(mt))).intersects(hitbox_)) {
      rt = mt;
    } else {
      lt = mt;
    }
  }
  return lt;
}

void PhysicalComponent::UpdateHitbox(double dt) {
  velocity_ += force_ / mass_;
  hitbox_ =  hitbox_ + velocity_ * static_cast<float>(dt);
}

void PhysicalComponent::RevertHitbox(double dt) {
  hitbox_ = hitbox_ + velocity_ * static_cast<float>(dt);
}

