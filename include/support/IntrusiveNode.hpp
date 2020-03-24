#ifndef EFFECTIVE_BROCOLLI_INTRUSIVE_NODE
#define EFFECTIVE_BROCOLLI_INTRUSIVE_NODE

#include <cassert>

template<typename T>
struct IntrusiveNode {
  using Node = IntrusiveNode;

public:
  /**
   * Is the node in any list.
   */
  bool IsLinked() const noexcept;

  /**
   * Link this node before given in list.
   */
  void LinkBefore(Node* next) noexcept;

  /**
   * Unlink this node from current list.
   */
  void Unlink() noexcept;

  /**
   * @return pointer to the item held inside the node.
   */
  T* AsItem() noexcept;

//private:
  Node* prev = nullptr;
  Node* next = nullptr;
};

template<typename T>
bool IntrusiveNode<T>::IsLinked() const noexcept {
  return next != nullptr;
}

template<typename T>
void IntrusiveNode<T>::LinkBefore(Node* next) noexcept {
  assert(!IsLinked());

  prev = next->prev;
  prev->next = this;
  next = next;
  next->prev = this;
}

template<typename T>
void IntrusiveNode<T>::Unlink() noexcept {
  if (next) {
    next->prev = prev;
  }
  if (prev) {
    prev->next = next;
  }

  next = prev = nullptr;
}

template<typename T>
T* IntrusiveNode<T>::AsItem() noexcept {
  return static_cast<T*>(this);
}

#endif
