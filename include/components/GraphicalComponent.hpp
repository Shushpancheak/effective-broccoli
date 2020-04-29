#ifndef EFFECTIVE_BROCOLLI_GRAPHIC_COMPONENT_HPP
#define EFFECTIVE_BROCOLLI_GRAPHIC_COMPONENT_HPP

#include "Component.hpp"
#include "SFML/Graphics.hpp"

struct GraphicalComponent : public Component {
  static const ComponentID type_id = COMPONENT_GRAPHICAL;

  GraphicalComponent(EntityID owner, GraphicalComponent* const parent);

  // uint8_t animation_variant;
  // uint8_t animation_frame;

  sf::Sprite sprite;
  sf::Transform abs_transform; // absolute transform that
                               // equals parents_transform * rel_transform
  sf::Transform rel_transform; // local transform that applies to the parent's transform

  std::vector<GraphicalComponent*> children;
};


#endif //EFFECTIVE_BROCOLLI_GRAPHIC_COMPONENT_HPP
