#ifndef EFFECTIVE_BROCOLLI_GRAPHIC_COMPONENT_HPP
#define EFFECTIVE_BROCOLLI_GRAPHIC_COMPONENT_HPP

#include "Component.hpp"
#include "SFML/Graphics.hpp"

struct GraphicalComponent : public Component {
  static const ComponentID type_id = COMPONENT_GRAPHICAL;

  GraphicalComponent(EntityID owner, GraphicalComponent* const parent);

  // uint8_t animation_variant;
  // uint8_t animation_frame;  - later do svyazi

  sf::Sprite sprite;           // contains (of all other things) absolute transform
  sf::Transform rel_transform; // local transform that applies to the parent's transform

  std::vector<GraphicalComponent*> children;
};


#endif //EFFECTIVE_BROCOLLI_GRAPHIC_COMPONENT_HPP
