//
// Created by dm on 3/23/20.
//

#ifndef EFFECTIVE_BROCOLLI_INCLUDE_PHYSICALCOMPONENT_HPP_
#define EFFECTIVE_BROCOLLI_INCLUDE_PHYSICALCOMPONENT_HPP_
#include "SFML/Graphics/Rect.hpp"
#include "Component.hpp"
#include "support/time.hpp"

#include "engine/core.hpp"

enum class PhysicalGroup {
  StaticObject,
  DynamicObject,
  UnInteractObject
};

class PhysicalComponent : public Component {
 public:
  static const ComponentID type_id = COMPONENT_PHYSICAL;
  friend class PhysicalSystem;

  PhysicalComponent(const EntityID owner, sf::Rect<float> hitbox_, float mass_, PhysicalGroup group);

 //private:
  static constexpr double EPS = 1e-7;
  sf::Rect<float> hitbox_;
  sf::Vector2f velocity_;
  float mass_;
  sf::Vector2f force_;
  PhysicalGroup group_;
};

inline sf::FloatRect operator+(sf::FloatRect box, const sf::Vector2f& ds) {
  box.top += ds.y;
  box.left += ds.x;
  return box;
}


#endif //EFFECTIVE_BROCOLLI_INCLUDE_PHYSICALCOMPONENT_HPP_
