#ifndef BROCOLLI_ENTITY
#define BROCOLLI_ENTITY
#include <vector>

#ifndef WIN32
#include <cstddef>
#endif

class Entity {
public:
  Entity();

  Entity(const Entity& other);
  Entity(Entity&& other) noexcept;

  Entity& operator=(const Entity& other);
  Entity& operator=(Entity&& other) noexcept;

  ~Entity() = default;

private:
  size_t id_;
  // Current available entity ID.
  static size_t current_id_;

  // For pooling
  bool in_use_;
  //TODO last time in use? delete it if it was too long ago.
};

#endif // BROCOLLI_ENTITY
