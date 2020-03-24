#ifndef EFFECTIVE_BROCOLLI_GRAPHICCOMPONENT_HPP
#define EFFECTIVE_BROCOLLI_GRAPHICCOMPONENT_HPP

#include "Component.hpp"
#include "SFML/Graphics.hpp"

struct GraphicComponent : Component<GraphicComponent> {
  float x, y;
  sf::Color color;
};


#endif //EFFECTIVE_BROCOLLI_GRAPHICCOMPONENT_HPP
