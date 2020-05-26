#ifndef EFFECTIVE_BROCOLLI_AI_COMPONENT_HPP
#define EFFECTIVE_BROCOLLI_AI_COMPONENT_HPP

#include "Component.hpp"
#include <vector>

enum class AiType {
  AGGRESSIVE
};

struct AiComponent : public Component {
  static const ComponentID type_id = COMPONENT_AI;

  AiComponent(EntityID owner, AiType type);

  AiType type;
};

inline AiComponent::AiComponent(EntityID owner, AiType type)
  : Component(owner)
  , type(type) {}


#endif
