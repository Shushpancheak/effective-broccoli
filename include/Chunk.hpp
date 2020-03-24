#ifndef EFFECTIVE_BROCOLLI_CHUNK
#define EFFECTIVE_BROCOLLI_CHUNK

#include <utility>
#include <cassert>

namespace objects {

enum ChunkErrorCodes {
  FALSE_TYPE,
  CTOR_FAILED,
  BAD_PTR
};

using TypeID = size_t;

/**
 * Chunk of objects of certain type, that is decided at runtime.
 * Use Add and Delete to manage objects.
 *
 * The type of items stored must have a static type ID
 * in field of type size_t named type_id.
 *
 * The type stored also has to be at least one word long.
 */
template<size_t ObjectsInChunk = 10>
class Chunk {
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
   * Deletes the object pointed to by item_ptr, using T's destructor.
   */
  template<typename T>
  int Delete(T* item_ptr);

private:
  /**
   * Construct the item of type T with args.
   *
   * @return error codes:
   * FALSE_TYPE  - type T does not correspond to the static type_id given.
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
};

template <size_t ObjectsInChunk>
Chunk<ObjectsInChunk>::Chunk(
  const size_t object_size,
  const TypeID type_id)
  : object_size_(object_size)
  , type_id_(type_id) {
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
  if (available_ptr == nullptr) {
    return nullptr;
  }

  assert(available_ptr >= buffer_ &&
         available_ptr <= buffer_ + ObjectsInChunk * object_size_);

  const int res = Construct<T, Args...>(available_ptr, std::forward(args)...);

  return res == 0 ? available_ptr : nullptr;
}

template<size_t ObjectsInChunk>
template<typename T>
int Chunk<ObjectsInChunk>::Delete(T* item_ptr) {
  assert(reinterpret_cast<void*>(item_ptr) >= buffer_ &&
         reinterpret_cast<void*>(item_ptr) <= buffer_ +
                                              ObjectsInChunk * object_size_);

  item_ptr->~T();

  Free(item_ptr, 1);

  return 0;
}

// - - - - - - - - - - - - - - - - PRIVATES - - - - - - - - - - - - - - - - - -

template<size_t ObjectsInChunk>
template<typename T, typename ... Args>
int Chunk<ObjectsInChunk>::Construct(void* item_addr, Args&&... args) {
  assert(item_addr >= buffer_ &&
         item_addr <= buffer_ + ObjectsInChunk * object_size_);

  if (T::type_id != type_id_) {
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