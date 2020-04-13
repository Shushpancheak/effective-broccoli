#ifndef EFFECTIVE_BROCOLLI_GRAPHIC_COMPONENT_HPP
#define EFFECTIVE_BROCOLLI_GRAPHIC_COMPONENT_HPP

#include "Component.hpp"
#include "SFML/Graphics.hpp"

struct GraphicComponent : Component<GraphicComponent> {
  float x = 0, y = 0;
  sf::Color color;
};


#endif //EFFECTIVE_BROCOLLI_GRAPHIC_COMPONENT_HPP
