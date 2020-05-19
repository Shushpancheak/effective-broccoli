#include "components/TransformComponent.hpp"

const ComponentID TransformComponent::type_id;

TransformComponent::TransformComponent(EntityID owner, TransformComponent* const parent)
  : Component(owner) {
  if (!parent) {
    return;
  }
  parent->children.push_back(this);
}
