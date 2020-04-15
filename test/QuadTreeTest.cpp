//
// Created by dm on 3/24/20.
//

#include "SFML/Graphics.hpp"
#include "gtest/gtest.h"
#include "support/QuadTree.hpp"
#include <iostream>

TEST(QuadTree, InstesectTest) {
  QuadTree<int> tree(sf::FloatRect{0, 0, 100, 100});
  sf::FloatRect a(1, 1, 2, 2);
  sf::FloatRect b(2, 2, 2, 2);
  int a_ = 1, b_ = 2;
  tree.insert(a, &a_);
  tree.insert(b, &b_);
  ASSERT_TRUE(tree.findAllIntersections().size() == 1);
}

TEST(QuadTree, NotIntersectTest) {
  QuadTree<int> tree(sf::FloatRect{0, 0, 100, 100});
  sf::FloatRect a(1, 1, 2, 2);
  sf::FloatRect b(4, 4, 2, 2);
  int a_ = 1, b_ = 2;
  tree.insert(a, &a_);
  tree.insert(b, &b_);
  ASSERT_TRUE(tree.findAllIntersections().size() == 0);
}
