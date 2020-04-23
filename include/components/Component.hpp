#ifndef EFFECTIVE_BROCOLLI_COMPONENT_HPP
#define EFFECTIVE_BROCOLLI_COMPONENT_HPP

#include "support/typedefs.hpp"

enum {
  COMPONENT_NULL = 0,
  COMPONENT_GRAPHICAL,
  COMPONENT_TRANSFORM,
  COMPONENT_PHYSICAL,
  COMPONENT_MAX
};

class Component {
public:
  // static const ComponentID type_id; -- Declare in children

  explicit Component(const EntityID owner);

  EntityID entity_id;
};

inline Component::Component(const EntityID owner)
  : entity_id(owner) {}

#endif //EFFECTIVE_BROCOLLI_COMPONENT_HPP
