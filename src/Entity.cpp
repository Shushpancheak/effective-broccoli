#include "Entity.hpp"

Entity::Entity()
  : id_(current_id_++)
  , in_use_(true) {}

Entity::Entity(const Entity& other)
  : id_(current_id_++)
  , in_use_(true) {}

Entity::Entity(Entity&& other) noexcept
  : id_(other.id_)
  , in_use_(true) {}

Entity& Entity::operator=(const Entity& other) {
  if (this == &other) {
    return *this;
  }

  id_     = current_id_++;
  in_use_ = true;
  return *this;
}

Entity& Entity::operator=(Entity&& other) noexcept {
  id_ = other.id_;
  return *this;
}
