#include "Component.hpp"
#include "ComponentManager.hpp"
#include "constants/error.hpp"

struct TestComponent : Component<TestComponent> {};

TEST(Error, NotFound) {
  ComponentManager test;
  auto res = test.GetComponent<TestComponent>(0);
  ASSERT_TRUE(res.HasError());
  ASSERT_EQ(res.Error().value(), NOT_FOUND);
  ASSERT_EQ(res.Error().message(), "Object was not found.");
  (void)test.AddComponent<TestComponent>(0);
  res = test.GetComponent<TestComponent>(0);
  ASSERT_TRUE(res.IsOk());
  ASSERT_NE(res.Value(), nullptr);
}
