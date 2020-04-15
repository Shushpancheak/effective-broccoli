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
  tree.insert(a, a_);
  tree.insert(b, b_);
  ASSERT_TRUE(tree.findAllIntersections().size() == 1);
}

TEST(QuadTree, NotIntersectTest) {
  QuadTree<int> tree(sf::FloatRect{0, 0, 100, 100});
  sf::FloatRect a(1, 1, 2, 2);
  sf::FloatRect b(3, 3, 2, 2);
  int a_ = 1, b_ = 2;
  tree.insert(a, a_);
  tree.insert(b, b_);
  ASSERT_TRUE(tree.findAllIntersections().size() == a.intersects(b));
}

TEST(QuadTree, IntersectWithTwo) {
  QuadTree<int> tree(sf::FloatRect{0, 0, 100, 100});
  sf::FloatRect a(1, 1, 2, 2);
  sf::FloatRect b(3, 3, 2, 2);
  sf::FloatRect c(2, 2, 2, 2);
  int a_ = 1, b_ = 2, c_ = 3;
  tree.insert(a, a_);
  tree.insert(b, b_);
  tree.insert(c, c_);
  ASSERT_TRUE(tree.findAllIntersections().size() == 2);
}

TEST(QuadTree, StressTest) {
  const int max_coord = 1000;
  QuadTree<int> tree(sf::FloatRect{0,0, max_coord + 1, max_coord + 1});

  const int obj_count = 100;
  std::vector<sf::FloatRect> rects;
  for (int i = 0; i < obj_count; i++) {
    int x_coord = rand() % max_coord;
    int y_coord = rand() % max_coord;
    rects.emplace_back(x_coord, y_coord, rand() % (max_coord - x_coord), rand() % (max_coord - y_coord));
    tree.insert(rects.back(), i);
  }

  size_t all_inter = 0;
  for (const auto& f_ : rects)
    for (const auto &s_ : rects)
      all_inter += f_.intersects(s_);
  all_inter -= obj_count;
  all_inter /= 2;

  ASSERT_EQ(tree.findAllIntersections().size(), all_inter);
}