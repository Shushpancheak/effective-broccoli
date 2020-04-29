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
  for (int it = 0; it < 1; it++) {
    const int max_coord = 100;
    QuadTree<int> tree(sf::FloatRect{0, 0, max_coord + 1, max_coord + 1});
    srand(time(NULL));
    const int obj_count = 100;
    std::vector<sf::FloatRect> rects;
    for (int i = 0; i < obj_count; i++) {
      int x_coord = rand() % max_coord;
      int y_coord = rand() % max_coord;
      rects.emplace_back(x_coord, y_coord, rand() % (max_coord - x_coord), rand() % (max_coord - y_coord));
      tree.insert(rects.back(), i);
    }
    auto intersections = tree.findAllIntersections();
    size_t all_inter = 0;
    for (int i = 0; i < obj_count; i++)
      for (int j = i + 1; j < obj_count; j++) {
        if (rects[i].intersects(rects[j])) {
          if (std::find(std::begin(intersections), std::end(intersections), std::pair(i, j)) == std::end(intersections) &&
              std::find(std::begin(intersections), std::end(intersections), std::pair(j, i)) == std::end(intersections)) {
            std::cout << rects[i] << rects[j] << std::endl;
            ASSERT_TRUE(false);
          }
        }
      }
  }
}