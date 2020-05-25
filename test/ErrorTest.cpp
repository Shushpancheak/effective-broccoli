#include "components/Component.hpp"
#include "managers/ComponentManager.hpp"
#include "constants/error.hpp"
#include "components/GraphicalComponent.hpp"

struct TestComponent : Component {
  static const ComponentID type_id = 1;
  size_t padding = 0;

  TestComponent(const EntityID owner)
    : Component(owner) {}
};

TEST(Error, NotFound) {
  ComponentManager test;
  auto res = test.GetComponent<GraphicalComponent>(0);
  ASSERT_TRUE(res.HasError());
  ASSERT_EQ(res.Error().value(), NOT_FOUND);
  ASSERT_EQ(res.Error().message(), "Object not found.");
  (void)test.AddComponent<GraphicalComponent>(0, nullptr);
  res = test.GetComponent<GraphicalComponent>(0);
  ASSERT_TRUE(res.IsOk());
  ASSERT_NE(res.Value(), nullptr);
}
