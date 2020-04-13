//
// Created by dm on 3/23/20.
//

#include "QuadTree.hpp"



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
  if (inner_box.left < outer_box.left + outer_box.width / 2) {
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

void QuadTree::split(QuadTree::pNode node, const sf::FloatRect& box) {
  for (int i = 0; i < 4; i++) {
    node->children[i] = new Node();
  }
  std::vector<PhysicalComponent*> new_components;
  for (auto& component : node->values) {
    int index = ComputeIndexByBox(box, component->GetHitbox());
    if (index == -1)
      new_components.push_back(component);
    else
      node->children[index]->values.push_back(component);
  }
  node->values = std::move(new_components);
}

void QuadTree::merge(QuadTree::pNode node) {
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

void QuadTree::insert(QuadTree::Node *node, int depth, const sf::FloatRect &box, PhysicalComponent *new_component) {
  if (isLeaf(node)) {
    if (depth >= MaxDepth || node->values.size() < Threshold)
      node->values.push_back(new_component);
    else {
      split(node, box);
      insert(node, depth, box, new_component);
    }
  } else {
    int index = ComputeIndexByBox(box, new_component->GetHitbox());
    if (index == -1) {
      node->values.push_back(new_component);
    } else {
      insert(node->children[index], depth + 1, ComputeBoxByIndex(box, index), new_component);
    }
  }
}
void QuadTree::erase(QuadTree::pNode node, QuadTree::pNode parent, sf::FloatRect box, PhysicalComponent *value) {
  if (isLeaf(node)) {
    for (auto& component: node->values) {
      if (component == value) {
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
        if (component == value) {
          std::swap(component, node->values.back());
          node->values.pop_back();
        }
      }
    } else {
      erase(node->children[index], node, ComputeBoxByIndex(box, index), value);
    }
  }
}

void QuadTree::FindAllIntersections(QuadTree::pNode node, std::vector<std::pair<PhysicalComponent*, PhysicalComponent*> >& intersections) {
  int max_val = 0;
  for (auto& value : node->values) {
    for (int j = 0; j < max_val; j++)
      if (value->GetHitbox().intersects(node->values[j]->GetHitbox()));
    max_val++;
  }
  if (!isLeaf(node)) {
    for (const auto& child: node->children) {
      for (const auto& value: node->values)
        FindAllDescIntersections(child, value, intersections);
    }
    for (const auto& child: node->children)
      FindAllIntersections(child, intersections);
  }
}

void QuadTree::FindAllDescIntersections(QuadTree::pNode node, PhysicalComponent *component, std::vector<std::pair<PhysicalComponent*, PhysicalComponent*> >& intersections) {
  for (const auto& other : node->values)
    if (component->GetHitbox().intersects(other->GetHitbox()))
      intersections.emplace_back(component, other);
  if (!isLeaf(node))
    for (const auto& child : node->children)
      FindAllDescIntersections(child, component, intersections);
}