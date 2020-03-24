#include "gtest/gtest.h"
#include "ObjectPool.hpp"
#include "Component.hpp"

class FakeInt {
public:
  static size_t id;
  static size_t GetTypeID() { return id; }
  FakeInt(size_t data) : data_(data) { }
private:
  size_t data_;
};

size_t FakeInt::id = 0;

TEST(ObjectPool, CreateSimple) {
  ObjectPool test;
  auto first =  test.CreateObject<FakeInt>(7);
  auto second = test.CreateObject<FakeInt>(42);
  ASSERT_NE(first, second);
  EXPECT_EQ(first + sizeof(FakeInt), second);
}
