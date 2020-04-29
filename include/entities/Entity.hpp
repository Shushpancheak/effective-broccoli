#ifndef BROCOLLI_ENTITY
#define BROCOLLI_ENTITY
#include <vector>

#ifndef WIN32
#include <cstddef>
#endif

#include "support/typedefs.hpp"

enum {
  ENTITY_NULL = 0,

  ENTITY_GRAPHICAL_PLACEHOLDER,
  ENTITY_BROCCOLI,

  ENTITY_MAX
};

class Entity {
public:
  static const EntityID type_id = ENTITY_NULL; // -- add in children.

  explicit Entity(const EntityID id)
    : id_(id) {};

  ~Entity() = default;

  EntityID GetEntityID() const { return id_; }

  void operator delete(void*) = delete;
  void operator delete[](void*) = delete;

private:
  EntityID id_;
};

#endif // BROCOLLI_ENTITY
