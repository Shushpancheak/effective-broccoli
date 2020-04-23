#include "components/TransformComponent.hpp"

TransformComponent::TransformComponent(EntityID owner, TransformComponent* const parent)
  : Component(owner) {
  if (!parent) {
    return;
  }
  parent->children.push_back(this);
}
