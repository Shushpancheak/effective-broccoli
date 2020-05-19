#include "components/GraphicalComponent.hpp"

const ComponentID GraphicalComponent::type_id;

GraphicalComponent::GraphicalComponent(EntityID owner, GraphicalComponent* const parent)
  : Component(owner) {
  if (!parent) {
    return;
  }
  parent->children.push_back(this);
}
