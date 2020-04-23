#ifndef EFFECTIVE_BROCOLLI_TRANSFORM_COMPONENT_HPP
#define EFFECTIVE_BROCOLLI_TRANSFORM_COMPONENT_HPP

#include "Component.hpp"
#include <vector>
#include "SFML/Graphics/Transform.hpp"

struct TransformComponent : public Component {
  static const ComponentID type_id = COMPONENT_TRANSFORM;

  TransformComponent(EntityID owner, TransformComponent* const parent);

  sf::Transform abs_transform;
  sf::Transform rel_transform;

  std::vector<TransformComponent*> children;
};


#endif
