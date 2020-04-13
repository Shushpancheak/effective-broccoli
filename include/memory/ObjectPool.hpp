#ifndef EFFECTIVE_BROCOLLI_OBJECT_POOL
#define EFFECTIVE_BROCOLLI_OBJECT_POOL

#include "constants/error.hpp"
#include "support/IntrusiveList.hpp"
#include "memory/DataChunk.hpp"

class ObjectPool {
  using Chunk = DataChunk;

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
  Status DeleteObject(T* obj_ptr);

  /**
   * As opposed to DeleteObject<T>, does not call a destructor, but simply
   * rewrites memory pointed to by data_ptr, thus making the space available
   * for using later.
   *
   * @returb Error Codes:
   * NOT_FOUND - data pointed to by data_ptr is not found in any of chunks.
   */
  Status Free(void* data_ptr);

private:
  IntrusiveList<DataChunk> chunks_;
};

template<typename T, typename ... Args>
T* ObjectPool::CreateObject(Args&&... args) {
  for (auto& chunk : chunks_) {
    if (chunk.GetTypeID() == T::GetTypeID() && !chunk.IsFull()) {
      T* res_ptr = chunk.Add<T>(std::forward<Args>(args)...);
      if (res_ptr != nullptr) {
        return res_ptr;
      }
    }
  }

  Chunk* new_chunk = new Chunk(sizeof(T), T::GetTypeID());

  chunks_.PushFront(new_chunk);
  return chunks_.begin()->Add<T>(std::forward<Args>(args)...);
}

template<typename T>
Status ObjectPool::DeleteObject(T* obj_ptr) {
  for (auto& chunk : chunks_) {
    if (chunk.GetPresentStatus(obj_ptr).IsOk()) {
      chunk.Delete<T>(obj_ptr);
      return make_result::Ok();
    }
  }

  return make_result::Fail(NOT_FOUND);
}

#endif // EFFECTIVE_BROCOLLI_OBJECT_POOL
