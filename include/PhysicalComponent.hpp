//
// Created by dm on 3/23/20.
//

#ifndef EFFECTIVE_BROCOLLI_INCLUDE_PHYSICALCOMPONENT_HPP_
#define EFFECTIVE_BROCOLLI_INCLUDE_PHYSICALCOMPONENT_HPP_
#include "SFML/Graphics/Rect.hpp"
#include <memory>

class PhysicalComponent {
 public:
  PhysicalComponent(sf::Rect<float> hitbox_, float mass_, int group);
  ~PhysicalComponent();
  const sf::FloatRect &GetHitbox();
 private:
  sf::Rect<float> hitbox_;
  sf::Vector2f velocity_;
  float mass_;
  int group_;
};
#endif //EFFECTIVE_BROCOLLI_INCLUDE_PHYSICALCOMPONENT_HPP_
