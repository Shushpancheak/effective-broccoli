#ifndef BROCOLLI_ENTITY
#define BROCOLLI_ENTITY
#include <vector>

#ifndef WIN32
#include <cstddef>
#endif

#include "support/typedefs.hpp"

class Entity  {
public:
  // static const EntityID type_id; -- add in children.

  explicit Entity(const EntityID id)
    : id_(id) {};

  //Entity(const Entity& other) = delete;
  //Entity(Entity&& other) noexcept;
  //
  //Entity& operator=(const Entity& other) = delete;
  //Entity& operator=(Entity&& other) noexcept;

  ~Entity() = default;

  EntityID GetEntityID() const { return id_; }

  void operator delete(void*) = delete;
  void operator delete[](void*) = delete;

private:
  EntityID id_;
};

#endif // BROCOLLI_ENTITY
