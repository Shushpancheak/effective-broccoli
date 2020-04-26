#ifndef EFFECTIVE_BROCCOLI_VALUE_STORAGE_HPP
#define EFFECTIVE_BROCCOLI_VALUE_STORAGE_HPP

template <typename T>
class ValueStorage {
  using Storage = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

public:
  template <typename... Arguments>
  void Construct(Arguments&&... arguments) {
    new (&storage_) T(std::forward<Arguments>(arguments)...);
  }

  void MoveConstruct(T&& that) {
    new (&storage_) T(std::move(that));
  }

  void CopyConstruct(const T& that) {
    new (&storage_) T(that);
  }

  T* PtrUnsafe() {
    return reinterpret_cast<T*>(&storage_);
  }

  const T* PtrUnsafe() const {
    return reinterpret_cast<const T*>(&storage_);
  }

  void Destroy() {
    PtrUnsafe()->~T();
  }

private:
  Storage storage_;
};

#endif // EFFECTIVE_BROCCOLI_VALUE_STORAGE_HPP
