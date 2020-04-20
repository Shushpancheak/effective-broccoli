#ifndef EFFECTIVE_BROCOLLI_COMPONENT_HPP
#define EFFECTIVE_BROCOLLI_COMPONENT_HPP

#include "support/typedefs.hpp"

class Component {
public:
  // static const ComponentID type_id; -- Declare in children

  size_t GetOwner() const {
    return entity_id_;
  }

  void operator delete(void*) = delete;
  void operator delete[](void*) = delete;

protected:
  EntityID entity_id_ = 0;
};

enum {
  COMPONENT_GRAPHICAL = 0,
  COMPONENT_PHYSICAL,
  COMPONENT_MAX
};

#endif //EFFECTIVE_BROCOLLI_COMPONENT_HPP
