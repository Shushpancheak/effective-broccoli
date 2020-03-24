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

private:
  Node* prev_ = nullptr;
  Node* next_ = nullptr;
};

template<typename T>
bool IntrusiveNode<T>::IsLinked() const noexcept {
  return next != nullptr;
}

template<typename T>
void IntrusiveNode<T>::LinkBefore(Node* next) noexcept {
  assert(!IsLinked());

  prev_ = next->prev_;
  prev_->next_ = this;
  next_ = next;
  next->prev_ = this;
}

template<typename T>
void IntrusiveNode<T>::Unlink() noexcept {
  if (next_) {
    next_->prev_ = prev_;
  }
  if (prev_) {
    prev_->next_ = next_;
  }

  next_ = prev_ = nullptr;
}

template<typename T>
T* IntrusiveNode<T>::AsItem() noexcept {
  return static_cast<T*>(this);
}

#endif
