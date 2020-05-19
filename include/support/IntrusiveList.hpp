#ifndef EFFECTIVE_BROCOLLI_INTRUSIVE_LIST
#define EFFECTIVE_BROCOLLI_INTRUSIVE_LIST

#include "IntrusiveNode.hpp"
#include <iterator>

namespace intrusive_iterator {

template <class NodeT, class ItemT>
class IteratorImpl {
  using Iterator = IteratorImpl<NodeT, ItemT>;

public:
  using value_type = ItemT;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

public:
  IteratorImpl(NodeT* start, NodeT* end) : current_(start), end_(end) {
  }

  // Prefix increment
  Iterator& operator++() {
    current_ = current_->next_node;
    return *this;
  }

  bool operator==(const Iterator& that) const {
    return current_ == that.current_;
  }

  bool operator!=(const Iterator& that) const {
    return !(*this == that);
  }

  ItemT& operator*() const {
    return *Item();
  }

  ItemT* operator->() const {
    return Item();
  }

  ItemT* Item() const {
    return static_cast<ItemT*>(current_);
  }

private:
  NodeT* current_;
  NodeT* end_;
};

}

template<typename T>
class IntrusiveList {
  using Node = IntrusiveNode<T>;

public:
  using Iterator = intrusive_iterator::IteratorImpl<Node, T>;
  using ConstIterator = intrusive_iterator::IteratorImpl<const Node, const T>;

  IntrusiveList();
  ~IntrusiveList();
  IntrusiveList(IntrusiveList&& other) noexcept;
  IntrusiveList(const IntrusiveList& other) = delete;
  IntrusiveList& operator=(const IntrusiveList& other) = delete;
  IntrusiveList& operator=(IntrusiveList&& other) = delete;

  void PushFront(Node* node) noexcept;
  void PushBack(Node* node) noexcept;
  void Append(IntrusiveList& other);

  /**
   * @return nullptr on fail.
   */
  T* PopFront();
  /**
   * @return nullptr on fail.
   */
  T* PopBack();  

  /**
   * Pop all elements.
   */
  void Clear();

  bool IsEmpty() const noexcept;
  size_t Size() const;

  Iterator begin();
  Iterator end();
  ConstIterator begin() const;
  ConstIterator end() const;

private:
  void InitEmpty();

  // Fake node.
  Node head_;
};

template<typename T>
void IntrusiveList<T>::PushFront(Node* node) noexcept {
  node->LinkBefore(head_.next_node);
}

template<typename T>
void IntrusiveList<T>::PushBack(Node* node) noexcept {
  node->LinkBefore(head_);
}

template<typename T>
T* IntrusiveList<T>::PopFront() {
  if (IsEmpty()) {
    return nullptr;
  }

  Node* res = head_.next_node;
  res->Unlink();
  return res->AsItem();
}

template<typename T>
T* IntrusiveList<T>::PopBack() {
  if (IsEmpty()) {
    return nullptr;
  }

  Node* res = head_.prev_node;
  res->Unlink();
  return res->AsItem();
}

template<typename T>
void IntrusiveList<T>::Append(IntrusiveList& other) {
  if (other.IsEmpty()) {
    return;
  }

  Node* other_front = other.head_.next_node;
  Node* other_back = other.head_.prev_node;

  other_back->next_node = &head_;
  other_front->prev_node = head_.prev_node;

  Node* back = head_.prev_node;

  head_.prev_node = other_back;
  back->next_node = other_front;

  other.head_.next_node = other.head_.prev_node = &other.head_;
}

template<typename T>
size_t IntrusiveList<T>::Size() const {
  return std::distance(begin(), end());
}

template<typename T>
typename IntrusiveList<T>::Iterator IntrusiveList<T>::begin() {
  return Iterator(head_.next_node, &head_);
}

template<typename T>
typename IntrusiveList<T>::Iterator IntrusiveList<T>::end() {
  return Iterator(&head_, &head_);
}

template<typename T>
typename IntrusiveList<T>::ConstIterator IntrusiveList<T>::begin() const {
  return ConstIterator(head_.next_node, &head_);
}

template<typename T>
typename IntrusiveList<T>::ConstIterator IntrusiveList<T>::end() const {
  return ConstIterator(&head_, &head_);
}

template<typename T>
void IntrusiveList<T>::Clear() {
  Node* current = head_.next_node;
  while (current != &head_) {
    Node* next = current->next_node;
    current->Unlink();
    current = next;
  }
}

template<typename T>
bool IntrusiveList<T>::IsEmpty() const noexcept {
  return head_.next_node == &head_;
}

template<typename T>
IntrusiveList<T>::IntrusiveList() {
  InitEmpty();
}

template<typename T>
IntrusiveList<T>::~IntrusiveList() {
  Clear();
}

template<typename T>
IntrusiveList<T>::IntrusiveList(IntrusiveList&& other) noexcept {
  InitEmpty();
  Append(other);
}

template<typename T>
void IntrusiveList<T>::InitEmpty() {
  head_.next_node = head_.prev_node = &head_;
}

#endif
