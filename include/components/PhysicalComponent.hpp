//
// Created by dm on 3/23/20.
//

#ifndef EFFECTIVE_BROCOLLI_INCLUDE_PHYSICALCOMPONENT_HPP_
#define EFFECTIVE_BROCOLLI_INCLUDE_PHYSICALCOMPONENT_HPP_
#include "SFML/Graphics/Rect.hpp"
#include "Component.hpp"
#include "support/time.hpp"

enum class PhysicalGroup {
  StaticObject,
  DynamicObject,
  UnInteractObject
};

class PhysicalComponent : public Component<PhysicalComponent> {
 public:
  friend class PhysicalSystem;
  PhysicalComponent(sf::Rect<float> hitbox_, float mass_, PhysicalGroup group);

  sf::FloatRect GetHitbox();

  /**
   * Return time that need for first collision of object to our
   * @param moving_object
   * @return time requre for first collision
   */
  double trackCollision(const PhysicalComponent& object, Duration dt) const;

  /**
   * Update hitbox by implicit Euler integration
   * @param dt
   */
  void UpdateHitbox(double dt);

  /**
   * Revert hitbox by time dt.
   * @param dt
   */
  void revertHitbox(double dt);

 private:
  static constexpr double EPS = 1e-7;
  sf::Rect<float> hitbox_;
  sf::Vector2f velocity_;
  float mass_;
  sf::Vector2f force_;
  PhysicalGroup group_;
};

sf::FloatRect operator+(sf::FloatRect box, const sf::Vector2f& ds) {
  box.top += ds.y;
  box.left += ds.x;
  return box;
}


#endif //EFFECTIVE_BROCOLLI_INCLUDE_PHYSICALCOMPONENT_HPP_
