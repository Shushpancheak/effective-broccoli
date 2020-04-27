//
// Created by dm on 3/23/20.
//

#ifndef EFFECTIVE_BROCOLLI_INCLUDE_QUADTREE_HPP_
#define EFFECTIVE_BROCOLLI_INCLUDE_QUADTREE_HPP_
#include <memory>
#include <array>
#include "SFML/Graphics/Rect.hpp"

enum class QuadIndex {UpperLeft = 0, UpperRight, DownLeft, DownRight};

std::ostream& operator<<(std::ostream& stream, const sf::FloatRect &rect) {
  return stream << "{ (" << rect.left << ", " << rect.top << "), (" << rect.left + rect.width << ", " << rect.top + rect.height << ") }\n";
}

template <typename T>
class QuadTree
{
 public:

  explicit QuadTree(const sf::Rect<float>& box) :
      max_box_(box), mRoot(new Node()){}
  /**
   * Insert Component connected with some box in tree;
   * @param box
   * @param component
   */
  void insert(const sf::FloatRect &box, const T& component);

  /**
   * Delete Component from tree
   * @param box
   * @param value
   */
  void erase(const sf::FloatRect &box, const T& value);

  /**
   * Find all collisions in tree.
   * @return std::vector with collision pairs
   */
  auto findAllIntersections() const;

  auto findAllntersectionsWithBox(const sf::FloatRect &box) const;

 private:
  static constexpr size_t Threshold = 16;
  static constexpr size_t MaxDepth = 12;
  struct Element {
    T value;
    sf::FloatRect box;
    [[nodiscard]] sf::FloatRect getHitbox() const {return box;}

    Element(T value, const sf::FloatRect &box) : value(std::move(value)), box(box) {}
  };
  struct Node
  {
    std::array<Node*, 4> children = {nullptr};
    std::vector<Element> values;
  };

  sf::Rect<float> max_box_;

  using pNode = Node*;
  pNode mRoot;

  bool isLeaf(const pNode node) const
  {
    return !node->children[0];
  }

  void query(pNode node, const sf::FloatRect& box, const sf::FloatRect& query_box, std::vector<T>& result) const;

  void merge(pNode node);

  void split(pNode node, const sf::FloatRect& box);

  void insert(Node *node, int depth, const sf::FloatRect &outher_box, const Element& new_component);

  void erase(pNode node, pNode parent, sf::FloatRect box, const Element &value);

  void findAllIntersections(pNode node, std::vector<std::pair<T, T> >& int_vector) const;

  void findAllDescIntersections(QuadTree::pNode node, const Element &component, std::vector<std::pair<T, T> >& intersections) const;

  };

sf::Rect<float> ComputeBoxByIndex(sf::Rect<float> parent_box, int index) {
  switch ((QuadIndex) index) {
    case QuadIndex::UpperLeft: // Upper left
      return sf::FloatRect(parent_box.top, parent_box.left, parent_box.height / 2, parent_box.width / 2);
    case QuadIndex::UpperRight:
      return sf::FloatRect(parent_box.top, parent_box.left + parent_box.width / 2, parent_box.height / 2, parent_box.width / 2);
    case QuadIndex ::DownLeft:
      return sf::FloatRect(parent_box.top + parent_box.height / 2, parent_box.left, parent_box.height / 2, parent_box.width / 2);
    case QuadIndex::DownRight:
      return sf::FloatRect(parent_box.top + parent_box.height / 2, parent_box.left + parent_box.width / 2, parent_box.height / 2, parent_box.width / 2);
  }
}

int ComputeIndexByBox(const sf::FloatRect &outer_box, const sf::FloatRect &inner_box) {
  if (inner_box.left + inner_box.width < outer_box.left + outer_box.width / 2) {
    if (inner_box.top + inner_box.height < outer_box.top + outer_box.height / 2)
      return (int) QuadIndex::UpperLeft;
    else if (inner_box.top >= outer_box.top + outer_box.height / 2)
      return (int) QuadIndex::DownLeft;
    else
      return -1;
  } else if (inner_box.left >= outer_box.left + outer_box.width / 2) {
    if (inner_box.top + inner_box.height < outer_box.top + outer_box.height / 2)
      return (int) QuadIndex::UpperRight;
    else if (inner_box.top >= outer_box.top + outer_box.height / 2)
      return (int) QuadIndex::DownRight;
    else
      return -1;
  } else
    return -1;
}

template <typename T>
void QuadTree<T>::split(QuadTree::pNode node, const sf::FloatRect& box) {
  for (int i = 0; i < 4; i++) {
    node->children[i] = new Node();
  }
  std::vector<Element> new_values;
  for (auto& component : node->values) {
    int index = ComputeIndexByBox(box, component.getHitbox());
    if (index == -1) {
      new_values.push_back(component);
    }
    else {
      node->children[index]->values.push_back(component);
    }
  }
  node->values = std::move(new_values);
}

template <typename T>
void QuadTree<T>::merge(QuadTree::pNode node) {
  int components_count = 0;
  for (auto& child : node->children) {
    components_count += child->values.size();
  }
  components_count += node->values.size();
  if (components_count > Threshold) {
    return;
  }
  for (auto child: node->children) {
    node->values.insert(std::end(node->values), child->values.begin(), child->values.end());
    delete child;
  }

}

template <typename T>
void QuadTree<T>::insert(QuadTree::Node *node, int depth, const sf::FloatRect &outher_box, const Element& new_component) {
  if (isLeaf(node)) {
    if (depth >= MaxDepth || node->values.size() < Threshold) {
      node->values.push_back(new_component);
    } else {
      split(node, outher_box);
      insert(node, depth, outher_box, new_component);
    }
  } else {
    int index = ComputeIndexByBox(outher_box, new_component.getHitbox());
    if (index == -1) {
      node->values.push_back(new_component);
    } else {
      insert(node->children[index], depth + 1, ComputeBoxByIndex(outher_box, index), new_component);
    }
  }
}

template <typename T>
void QuadTree<T>::erase(QuadTree::pNode node, QuadTree::pNode parent, sf::FloatRect box, const Element &value) {
  if (isLeaf(node)) {
    for (auto& component: node->values) {
      if (component->value == value->value) {
        std::swap(component, node->values.back());
        node->values.pop_back();
      }
    }
    if (parent != nullptr)
      merge(parent);
  } else {
    int index = ComputeIndexByBox(box, value->GetHitbox());
    if (index == -1) {
      for (auto& component: node->values) {
        if (component->value == value->value) {
          std::swap(component, node->values.back());
          node->values.pop_back();
        }
      }
    } else {
      erase(node->children[index], node, ComputeBoxByIndex(box, index), value);
    }
  }
}

template <typename T>
void QuadTree<T>::findAllIntersections(QuadTree::pNode node, std::vector<std::pair<T, T> >& int_vector) const{
  int max_val = 0;
  for (auto& value : node->values) {
    for (int j = 0; j < max_val; j++)
      if (value.getHitbox().intersects(node->values[j].getHitbox()))
        int_vector.emplace_back(value.value, node->values[j].value);
    max_val++;
  }
  if (!isLeaf(node)) {
    for (const auto& child: node->children) {
      for (const auto& value: node->values)
        findAllDescIntersections(child, value, int_vector);
    }
    for (const auto& child: node->children)
      findAllIntersections(child, int_vector);
  }
}

template <typename T>
void QuadTree<T>::findAllDescIntersections(QuadTree::pNode node, const Element& component, std::vector<std::pair<T, T> >& intersections) const{
  for (const auto& other : node->values)
    if (component.getHitbox().intersects(other.getHitbox()))
      intersections.emplace_back(component.value, other.value);
  if (!isLeaf(node))
    for (const auto& child : node->children)
      findAllDescIntersections(child, component, intersections);
}

template <typename T>
auto QuadTree<T>::findAllIntersections() const{
  std::vector<std::pair<T, T> > result;
  findAllIntersections(mRoot, result);
  return result;
}

template<typename T>
void QuadTree<T>::erase(const sf::FloatRect &box, const T& value) {
  erase(mRoot, nullptr, max_box_, Element(value, box));
}

template <typename T>
void QuadTree<T>::insert(const sf::FloatRect &box, const T& component) {
  insert(mRoot, 0, max_box_, Element(component, box));
}
template<typename T>
void QuadTree<T>::query(QuadTree::pNode node,
                        const sf::FloatRect &box,
                        const sf::FloatRect &query_box,
                        std::vector<T> &result) const {
    for (const auto& value : node->values)
    {
      if (query_box.intersects(mGetBox(value.box)))
        result.push_back(value.value);
    }
    if (!isLeaf(node))
    {
      for (auto i = 0; i < node->children.size(); ++i)
      {
        auto childBox = ComputeBoxByIndex(box, i);
        if (query_box.intersects(childBox))
          query(node->children[i].get(), childBox, query_box, result);
      }
    }

}

template<typename T>
auto QuadTree<T>::findAllntersectionsWithBox(const sf::FloatRect &box) const {
  std::vector<T> result;
  query(mRoot, max_box_, box, result);
  return result;
}

#endif //EFFECTIVE_BROCOLLI_INCLUDE_QUADTREE_HPP_
