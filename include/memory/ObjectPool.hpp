#ifndef EFFECTIVE_BROCOLLI_OBJECT_POOL
#define EFFECTIVE_BROCOLLI_OBJECT_POOL

#include "constants/error.hpp"
#include "support/IntrusiveList.hpp"
#include "memory/DataChunk.hpp"
#include "support/result.hpp"

//TODO template<MaxTypeId>
class ObjectPool {
  const size_t DEFAULT_CHUNK_OBJECT_COUNT = 100;
  using Chunk = DataChunk;

public:
  ObjectPool();
  ~ObjectPool();

  ObjectPool(const ObjectPool&  other) = delete;
  ObjectPool(      ObjectPool&& other) = delete;
  ObjectPool& operator=(const ObjectPool&  other) = delete;
  ObjectPool& operator=(      ObjectPool&& other) = delete;

  /**
   * Create an object of type T with args.
   * type T must have a static type ID given in static field type_id.
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
  Result<T*> CreateObject(Args&&... args);

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

  /**
   * Set number of objects in following chunks.
   */
  void SetObjectCountInChunk(size_t count);

private:
  IntrusiveList<DataChunk> chunks_;
  size_t cur_chunks_obj_count_;
};

inline ObjectPool::ObjectPool()
  : chunks_()
  , cur_chunks_obj_count_(DEFAULT_CHUNK_OBJECT_COUNT) {}

inline void ObjectPool::SetObjectCountInChunk(size_t count) {
  cur_chunks_obj_count_ = count;
}

template<typename T, typename ... Args>
Result<T*> ObjectPool::CreateObject(Args&&... args) {
  static_assert(sizeof(T) >= sizeof(uint64_t)); // T must be at least one word long.
  static_assert((void)T::type_id); // T should have a static type_id field.

  for (auto& chunk : chunks_) {
    if (chunk.GetTypeID() == T::type_id && !chunk.IsFull()) {
      auto res_ptr = chunk.Add<T>(std::forward<Args>(args)...);
      if (res_ptr.Value() != nullptr) {
        return res_ptr;
      }
    }
  }

  Chunk* new_chunk = new Chunk(sizeof(T), T::type_id, cur_chunks_obj_count_);

  chunks_.PushFront(new_chunk);
  return chunks_.begin()->Add<T>(std::forward<Args>(args)...);
}

template<typename T>
Status ObjectPool::DeleteObject(T* obj_ptr) {
  for (auto& chunk : chunks_) {
    if (chunk.GetPresentStatus(obj_ptr).IsOk()) {
      return chunk.Delete<T>(obj_ptr);
    }
  }

  return make_result::Fail(NOT_FOUND);
}

#endif // EFFECTIVE_BROCOLLI_OBJECT_POOL
