//
// Created by dm on 3/23/20.
//

#ifndef EFFECTIVE_BROCOLLI_INCLUDE_QUADTREE_HPP_
#define EFFECTIVE_BROCOLLI_INCLUDE_QUADTREE_HPP_
#include <memory>
#include <array>
#include "SFML/Graphics/Rect.hpp"
#include "components/PhysicalComponent.hpp"
#include "systems/PhysicalSystem.hpp"

enum class QuadIndex {UpperLeft = 0, UpperRight, DownLeft, DownRight};

class QuadTree
{
 public:

  QuadTree(const sf::Rect<float>& box) :
      max_box_(box), mRoot(new Node()){}

 private:
  static constexpr size_t Threshold = 16;
  static constexpr size_t MaxDepth = 8;

  struct Node
  {
    std::array<Node*, 4> children = {nullptr};
    std::vector<PhysicalComponent*> values;
  };

  sf::Rect<float> max_box_;

  using pNode = Node*;
  pNode mRoot;

  bool isLeaf(const pNode node) const
  {
    return !node->children[0];
  }

  void merge(pNode node);

  void split(pNode node, const sf::FloatRect& box);

  void insert(Node *node, int depth, const sf::FloatRect &box, PhysicalComponent* new_component);

  void erase(pNode node, pNode parent, sf::FloatRect box, PhysicalComponent* value);

  void FindAllIntersections(pNode node, std::vector<std::pair<PhysicalComponent*, PhysicalComponent*> >& int_vector);

  void FindAllDescIntersections(QuadTree::pNode node, PhysicalComponent *component, std::vector<std::pair<PhysicalComponent*, PhysicalComponent*> >& intersections);

  };

#endif //EFFECTIVE_BROCOLLI_INCLUDE_QUADTREE_HPP_
