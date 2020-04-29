#ifndef EFFECTIVE_BROCCOLI_GRAPHICAL_PLACEHOLDER_ENTITY_HPP
#define EFFECTIVE_BROCCOLI_GRAPHICAL_PLACEHOLDER_ENTITY_HPP

#include "Entity.hpp"

class GraphicalPlaceholderEntity : public Entity {
public:
  static const EntityID type_id = ENTITY_GRAPHICAL_PLACEHOLDER;

  explicit GraphicalPlaceholderEntity(const EntityID id) : Entity(id) {}

private:
  volatile int padding {}; // To make the class one word long
};
#endif // EFFECTIVE_BROCCOLI_GRAPHICAL_PLACEHOLDER_ENTITY_HPP
