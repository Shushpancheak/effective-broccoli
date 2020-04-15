#ifndef BROCOLLI_ENTITY
#define BROCOLLI_ENTITY
#include <vector>

#ifndef WIN32
#include <cstddef>
#endif

#include "support/TypeCounter.hpp"

template <typename T>
class Entity  {
public:
  explicit Entity(size_t);

  Entity(const Entity& other) = delete;
  Entity(Entity&& other) noexcept;

  Entity& operator=(const Entity& other) = delete;
  Entity& operator=(Entity&& other) noexcept;

  ~Entity() = default;

  static const size_t type_id;
  static size_t GetTypeID();

  [[nodiscard]] size_t GetEntityID() const;

  void operator delete(void*) = delete;
  void operator delete[](void*) = delete;

private:
  size_t id_;

  // For pooling
  bool in_use_;
  //TODO last time in use? delete it if it was too long ago.
};

template <typename T>
const size_t Entity<T>::type_id = TypeCounter::GetId<T>();

#endif // BROCOLLI_ENTITY
