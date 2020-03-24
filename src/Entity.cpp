#include "Entity.hpp"

template <typename T>
Entity<T>::Entity(size_t id)
  : id_(id)
  , in_use_(true) {}

template <typename T>
Entity<T>::Entity(Entity<T>&& other) noexcept
  : id_(other.id_)
  , in_use_(true) {}


template <typename T>
Entity<T>& Entity<T>::operator=(Entity<T>&& other) noexcept {
  id_ = other.id_;
  return *this;
}
