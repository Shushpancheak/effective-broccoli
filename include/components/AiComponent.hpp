#ifndef EFFECTIVE_BROCOLLI_AI_COMPONENT_HPP
#define EFFECTIVE_BROCOLLI_AI_COMPONENT_HPP

#include "Component.hpp"
#include <vector>

enum class AiType {
  AGGRESSIVE
};

struct AiComponent : public Component {
  static const ComponentID type_id = COMPONENT_AI;

  AiComponent(EntityID owner, EntityID target, AiType type);
  EntityID target_id;
  AiType type;
};

inline AiComponent::AiComponent(EntityID owner, EntityID target, AiType type)
  : Component(owner), target_id(target)
  , type(type) {}


#endif
