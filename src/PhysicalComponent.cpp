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