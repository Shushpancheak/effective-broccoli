#include <iostream>
#include "gtest/gtest.h"
#include "ObjectPoolTest.cpp"
#include "ErrorTest.cpp"
#include "QuadTreeTest.cpp"

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}