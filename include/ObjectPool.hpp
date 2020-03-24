#ifndef EFFECTIVE_BROCOLLI_OBJECT_POOL
#define EFFECTIVE_BROCOLLI_OBJECT_POOL

#include "constants/error_codes.hpp"
#include "support/IntrusiveList.hpp"
#include "Chunk.hpp"

template<size_t ObjectsPerChunk = 10>
class ObjectPool {
  using Chunk = objects::Chunk<ObjectsPerChunk>;

public:
  ObjectPool() = default;
  ~ObjectPool();

  ObjectPool(const ObjectPool&  other) = delete;
  ObjectPool(      ObjectPool&& other) = delete;
  ObjectPool& operator=(const ObjectPool&  other) = delete;
  ObjectPool& operator=(      ObjectPool&& other) = delete;

  /**
   * Create an object of type T with args.
   * type T must have a static type ID given in static method GetTypeID().
   *
   * The created object may be deallocated in following cases:
   * 1) via DeleteObject<T>(ptr)
   * 2) on Destruction of ObjectPool.
   *    @warning in this case, destructors of objects
   *             still present in object pool won't be called.
   *
   * @return pointer to created object. Returns nullptr on failure.
   */
  template<typename T, typename... Args>
  T* CreateObject(Args&&... args);

  /**
   * Calls a destructor on object pointed to by obj_ptr and nullifies it.
   *
   * @return Error Codes:
   * NOT_FOUND - object pointed to by obj_ptr is not present in any of chunks.
   */
  template<typename T>
  int DeleteObject(T* obj_ptr);

  /**
   * As opposed to DeleteObject<T>, does not call a destructor, but simply
   * rewrites memory pointed to by data_ptr, thus making the space available
   * for using later.
   *
   * @returb Error Codes:
   * NOT_FOUND - data pointed to by data_ptr is not found in any of chunks.
   */
  int Free(void* data_ptr);

private:
  IntrusiveList<Chunk> chunks_;
};

template<size_t ObjectsPerChunk>
ObjectPool<ObjectsPerChunk>::~ObjectPool() {
  while (!chunks_.IsEmpty()) {
    Chunk* chunk = chunks_.PopFront();
    delete chunk;
  }
}

template<size_t ObjectsPerChunk>
template<typename T, typename ... Args>
T* ObjectPool<ObjectsPerChunk>::CreateObject(Args&&... args) {
  for (auto& chunk : chunks_) {
    if (chunk.GetTypeID() == T::GetTypeID() && !chunk.IsFull()) {
      T* res_ptr = chunk.template Add<T>(std::forward<Args>(args)...);
      if (res_ptr != nullptr) {
        return res_ptr;
      }
    }
  }

  Chunk* new_chunk = new Chunk(sizeof(T), T::GetTypeID());

  chunks_.PushFront(new_chunk);
  return chunks_.begin()->template Add<T>(std::forward<Args>(args)...);
}

template<size_t ObjectsPerChunk>
template<typename T>
int ObjectPool<ObjectsPerChunk>::DeleteObject(T* obj_ptr) {
  for (auto& chunk : chunks_) {
    if (chunk.GetPresentStatus(obj_ptr) == NO_ERROR) {
      chunk.template Delete<T>(obj_ptr);
      return NO_ERROR;
    }
  }

  return NOT_FOUND;
}

template<size_t ObjectsPerChunk>
int ObjectPool<ObjectsPerChunk>::Free(void* data_ptr) {
  for (auto& chunk : chunks_) {
    if (chunk.GetPresentStatus(data_ptr) == NO_ERROR) {
      chunk.HardDelete(data_ptr);
      return NO_ERROR;
    }
  }

  return NOT_FOUND;
}

#endif // EFFECTIVE_BROCOLLI_OBJECT_POOL
