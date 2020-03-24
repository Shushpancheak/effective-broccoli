#include "gtest/gtest.h"
#include "ObjectPool.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "Circle.hpp"

TEST(ObjectPool, CreateSimple) {
}

TEST(ObjectPool, CreateSimple1) {
  ObjectPool<> object_pool;

  for (size_t i = 0; i < 100; ++i) {
    object_pool.CreateObject<Circle>();
  }

}