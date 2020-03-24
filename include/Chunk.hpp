#ifndef EFFECTIVE_BROCOLLI_CHUNK
#define EFFECTIVE_BROCOLLI_CHUNK

#include <utility>
#include <cassert>
#include "support/IntrusiveList.hpp"
#include "constants/error_codes.hpp"

namespace objects {

using TypeID = size_t;

/**
 * Chunk of objects of certain type, that is decided at runtime.
 * Use Add and Delete to manage objects.
 *
 * The type of items stored must have a static type ID
 * given in static method GetTypeID().
 *
 * The type stored also has to be at least one word long.
 */
template<size_t ObjectsInChunk = 10>
class Chunk : public IntrusiveNode<Chunk<ObjectsInChunk>> {
public:
  explicit Chunk(const size_t object_size, const TypeID type_id);
  ~Chunk();

  Chunk(const Chunk& other) = delete;
  Chunk(Chunk&& other) = delete;
  Chunk& operator=(const Chunk& other) = delete;
  Chunk& operator=(Chunk&& other) = delete;

  /**
   * Emplaces the object with given args.
   *
   * @return Address of constructed object; nullptr on failure.
   */
  template<typename T, typename... Args>
  T* Add(Args&&... args);

  /**
   * Deletes the object pointed to by item_ptr, not using any destructor.
   * There may not even be an object, the deletion shall happen anyway.
   *
   * @warning DO NOT use with non-PODs.
   */
  void HardDelete(void* item_ptr);

  /**
   * Deletes the object pointed to by item_ptr, using T's destructor.
   */
  template<typename T>
  int Delete(T* item_ptr);

  /**
   * Deletes count objects of type T, starting from item_ptr, using T's destructor.
   */
  template<typename T>
  int Delete(T* item_ptr, size_t count);

  /**
   * Deletes the object pointed to by item_ptr, using T's destructor,
   * if an object does actually occupy this position.
   *
   * @return ALREADY_DELETED if the address is not occupied.
   */
  template<typename T>
  int DeleteIfPresent(T* item_ptr);

  /**
   * Deletes objects starting from item_ptr, using T's destructor,
   * if an object does actually occupy this position.
   *
   * @return ALREADY_DELETED if the address is not occupied.
   */
  template<typename T>
  int DeleteIfPresent(T* item_ptr, size_t count);

  /**
   * Deletes all objects in chunk, skipping uninitialized components,
   * thus clearing all chunk.
   */
  template<typename T>
  void DeleteAll();

  /**
   * @return NO_ERROR if item pointed to by item_ptr is present in chunk.
   * Error codes:
   * OUT_OF_BOUNDS      - Pointer is out of bounds of chunk.
   * OBJECT_NOT_PRESENT - There is no object in address.
   */
  template<typename T>
  int GetPresentStatus(T* item_ptr);

  /**
   * @return does chunk has no objects.
   */
  bool IsEmpty() const;

  /**
   * @return is chunk full with objects.
   */
  bool IsFull() const;

  /**
   * @return number of objects held in chunk.
   */
  size_t Size() const;

  /**
   * @return static type id of object type held in chunk.
   */
  TypeID GetTypeID() const;

private:
  /**
   * Construct the item of type T with args.
   *
   * @return error codes:
   * FALSE_TYPE  - type T does not correspond to the static GetTypeID given.
   * CTOR_FAILED - constructor of type T with args failed.
   * BAD_PTR     - item_addr is nullptr.
   */
  template<typename T, typename... Args>
  int Construct(void* item_addr, Args&&... args);

  /**
   * Free items starting from `item_addr` up to (T*)item_addr + count
   */
  void Free(void* item_addr, const size_t count);

  /**
   * Check if the item pointed to by `item_addr` is freed.
   */
  static bool IsAvailable(void* item_addr);

  /**
   * Get first available place where a new object can be placed.
   *
   * @return Address of the item; nullptr on failure.
   */
  void* GetAvailable() const;

private:
  static const size_t EMPTY_MEMORY_BYTES = 0xDEADBEEF;

  char* buffer_;
  const size_t object_size_;
  const TypeID type_id_;

  // Not really needed, but it can greatly optimize IsEmpty().
  size_t size_;
};

template <size_t ObjectsInChunk>
Chunk<ObjectsInChunk>::Chunk(
  const size_t object_size,
  const TypeID type_id)
  : object_size_(object_size)
  , type_id_(type_id)
  , size_(0) {
  buffer_ = new char[object_size * ObjectsInChunk];
  Free(buffer_, ObjectsInChunk);
}

template<size_t ObjectsInChunk>
Chunk<ObjectsInChunk>::~Chunk() {
  delete[] buffer_;
}

template<size_t ObjectsInChunk>
template<typename T, typename ... Args>
T* Chunk<ObjectsInChunk>::Add(Args&&... args) {
  const void* available_ptr = GetAvailable();
  if (available_ptr == nullptr || IsFull()) {
    return nullptr;
  }

  assert(available_ptr >= buffer_ &&
         available_ptr <= buffer_ + ObjectsInChunk * object_size_);

  const int res = Construct<T, Args...>(available_ptr, std::forward(args)...);

  if (res == 0) {
    ++size_;
    return available_ptr;
  }

  return nullptr;
}

template<size_t ObjectsInChunk>
void Chunk<ObjectsInChunk>::HardDelete(void* item_ptr) {
  Free(item_ptr, 1);
}

template<size_t ObjectsInChunk>
template<typename T>
int Chunk<ObjectsInChunk>::Delete(T* item_ptr) {
  assert(!IsEmpty());

  assert(reinterpret_cast<void*>(item_ptr) >= buffer_ &&
         reinterpret_cast<void*>(item_ptr) <= buffer_ +
                                              ObjectsInChunk * object_size_);

  item_ptr->~T();

  Free(item_ptr, 1);

  --size_;

  return NO_ERROR;
}

template<size_t ObjectsInChunk>
template<typename T>
void Chunk<ObjectsInChunk>::DeleteAll() {
  for (size_t i = 0; i < ObjectsInChunk; ++i) {
    DeleteIfPresent(buffer_ + i);
  }
}

template<size_t ObjectsInChunk>
template<typename T>
int Chunk<ObjectsInChunk>::GetPresentStatus(T* item_ptr) {
  if (item_ptr < buffer_ || item_ptr > buffer_ + ObjectsInChunk * object_size_) {
    return OUT_OF_BOUNDS;
  }

  if (IsAvailable(item_ptr)) {
    return OBJECT_NOT_PRESENT;
  } else {
    return NO_ERROR;
  }
}

template<size_t ObjectsInChunk>
template<typename T>
int Chunk<ObjectsInChunk>::Delete(T* item_ptr, size_t count) {
  for (size_t i = 0; i < count; ++i) {
    const int res = Delete(item_ptr + i);

    if (res != NO_ERROR) {
      return res;
    }
  }

  return NO_ERROR;
}

template<size_t ObjectsInChunk>
template<typename T>
int Chunk<ObjectsInChunk>::DeleteIfPresent(T* item_ptr) {
  if (IsAvailable(item_ptr)) {
    return ALREADY_DELETED;
  }

  return Delete(item_ptr);
}

template<size_t ObjectsInChunk>
template<typename T>
int Chunk<ObjectsInChunk>::DeleteIfPresent(T* item_ptr, size_t count) {
  for (size_t i = 0; i < count; ++i) {
    const int res = DeleteIfPresent(item_ptr + i);

    if (res != NO_ERROR) {
      return res;
    }
  }

  return NO_ERROR;
}

template<size_t ObjectsInChunk>
bool Chunk<ObjectsInChunk>::IsEmpty() const {
  return size_ == 0;
}

template<size_t ObjectsInChunk>
bool Chunk<ObjectsInChunk>::IsFull() const {
  return size_ == ObjectsInChunk;
}

template<size_t ObjectsInChunk>
size_t Chunk<ObjectsInChunk>::Size() const {
  return size_;
}

template<size_t ObjectsInChunk>
TypeID Chunk<ObjectsInChunk>::GetTypeID() const {
  return type_id_;
}

// - - - - - - - - - - - - - - - - PRIVATES - - - - - - - - - - - - - - - - - -

template<size_t ObjectsInChunk>
template<typename T, typename ... Args>
int Chunk<ObjectsInChunk>::Construct(void* item_addr, Args&&... args) {
  assert(item_addr >= buffer_ &&
         item_addr <= buffer_ + ObjectsInChunk * object_size_);

  if (T::GetTypeID() != type_id_) {
    return FALSE_TYPE;
  }

  if (item_addr == nullptr) {
    return BAD_PTR;
  }

  void* res_ptr = new (item_addr) T(std::forward<Args>(args)...);

  return (res_ptr == nullptr ? CTOR_FAILED : 0);
}

template<size_t ObjectsInChunk>
void* Chunk<ObjectsInChunk>::GetAvailable() const {
  for (size_t i = 0; i < ObjectsInChunk; ++i) {
    void* cur_addr = buffer_ + i * object_size_;
    if (IsAvailable(cur_addr)) {
      return cur_addr;
    }
  }

  return nullptr;
}

template<size_t ObjectsInChunk>
// ReSharper disable once CppMemberFunctionMayBeConst
void Chunk<ObjectsInChunk>::Free(void* item_addr, const size_t count) {
  assert(item_addr >= buffer_ &&
         item_addr <= buffer_ + ObjectsInChunk * object_size_);

  for (size_t i = 0; i < count; ++i) {
    *reinterpret_cast<size_t*>(item_addr + i * object_size_) =
      EMPTY_MEMORY_BYTES;
  }
}

template<size_t ObjectsInChunk>
bool Chunk<ObjectsInChunk>::IsAvailable(void* item_addr) {
  return *reinterpret_cast<size_t*>(item_addr) == EMPTY_MEMORY_BYTES;
}

}

#endif