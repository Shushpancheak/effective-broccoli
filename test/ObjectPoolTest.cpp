#include "gtest/gtest.h"
#include "memory/ObjectPool.hpp"
#include "Component.hpp"

namespace consts {
enum {
  FIRST_INT = 7,
  SECOND_INT = 42,

  STRESS_1_SIZE = 1000,
  STRESS_2_SIZE = 10'000,
  STRESS_3_SIZE = 100'000,
  STRESS_EQ_ASSERTIONS = false
};
}

class FakeInt {
public:
  static size_t id;
  static size_t GetTypeID() { return id; }
  explicit FakeInt(const int integer) : integer_(integer) {}

  operator int() const {
    return integer_;
  }

private:
  int integer_;
  int padding_for_pool_ = 0;
};

size_t FakeInt::id = 0;


TEST(ObjectPool, CreateFakeInt) {
  ObjectPool test;

  const auto first  = test.CreateObject<FakeInt>(FakeInt(7));
  const auto second = test.CreateObject<FakeInt>(consts::SECOND_INT);
  ASSERT_EQ(static_cast<int>(*first), consts::FIRST_INT);
  ASSERT_EQ(static_cast<int>(*second), consts::SECOND_INT);
  EXPECT_EQ(first + 1, second);
}


inline void stress_test(int size) {
  ObjectPool pool;
  std::vector<FakeInt*> int_ptrs(size);

  for (int i = 0; i < size; ++i) {
    int_ptrs[i] = pool.CreateObject<FakeInt>(i);
  }

  if constexpr (consts::STRESS_EQ_ASSERTIONS) {
    for (int i = 0; i < size; ++i) {
      ASSERT_EQ(static_cast<int>(*int_ptrs[i]), i);
    }
  }
}

TEST(ObjectPool, Stress1) { stress_test(consts::STRESS_1_SIZE); }
TEST(ObjectPool, Stress2) { stress_test(consts::STRESS_2_SIZE); }
TEST(ObjectPool, Stress3) { stress_test(consts::STRESS_3_SIZE); }
