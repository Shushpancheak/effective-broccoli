#include "components/GraphicalComponent.hpp"

GraphicalComponent::GraphicalComponent(EntityID owner, GraphicalComponent* const parent)
  : Component(owner) {
  if (!parent) {
    return;
  }
  parent->children.push_back(this);
}
