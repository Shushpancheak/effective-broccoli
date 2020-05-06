#ifndef EFFECTIVE_BROCOLLI_OBJECT_POOL
#define EFFECTIVE_BROCOLLI_OBJECT_POOL

#include "constants/error.hpp"
#include "support/IntrusiveList.hpp"
#include "memory/DataChunk.hpp"
#include "support/result.hpp"
#include "support/typedefs.hpp"

template<TypeID type_max>
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
   * @warning VERY slow. (iterates over all data chunks)
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
  IntrusiveList<DataChunk> lists_[type_max];
  size_t cur_chunks_obj_count_;

// Iterator
public:
  template<typename T>
  class Iterator {
  public:
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;
    using iterator_category = std::forward_iterator_tag;

    explicit Iterator(IntrusiveList<DataChunk>& chunks,
                      IntrusiveList<DataChunk>::Iterator cur_chunk)
      : list_(&chunks)
      , chunks_iter_(cur_chunk)
      , cur_chunk_iter_(chunks_iter_ == chunks.end()    ?
                        DataChunk::Iterator<T>(nullptr) :
                        chunks_iter_->begin<T>()) {}

    Iterator& operator++() {
      ++cur_chunk_iter_;
      if (cur_chunk_iter_ == chunks_iter_->end<T>()) {
        ++chunks_iter_;
        if (chunks_iter_ == list_->end()) {
          cur_chunk_iter_ = DataChunk::Iterator<T>(nullptr);
        } else {
          cur_chunk_iter_ = chunks_iter_->begin<T>();
        }
      }
      
      return *this;
    }

    Iterator operator++(int) {
      Iterator<T> iter = *this;
      ++(*this);
      return iter;
    }

    bool operator==(const Iterator& other) const {
      return cur_chunk_iter_ == other.cur_chunk_iter_;
    }

    bool operator!=(const Iterator& other) const {
      return cur_chunk_iter_ != other.cur_chunk_iter_;
    }

    T operator*() {
      return *cur_chunk_iter_;
    }

    T* operator->() {
      return cur_chunk_iter_.operator->();
    }

  private:
    IntrusiveList<DataChunk>* list_;
    IntrusiveList<DataChunk>::Iterator chunks_iter_;
    DataChunk::Iterator<T> cur_chunk_iter_;
  };

  template<typename T>
  Iterator<T> begin() {
    return Iterator<T>(lists_[T::type_id], lists_[T::type_id].begin());
  }

  template<typename T>
  Iterator<T> end() {
    return Iterator<T>(lists_[T::type_id], lists_[T::type_id].end());
  }
};

template<TypeID type_max>
inline ObjectPool<type_max>::ObjectPool()
  : lists_{}
  , cur_chunks_obj_count_(DEFAULT_CHUNK_OBJECT_COUNT) {}

template<TypeID type_max>
inline void ObjectPool<type_max>::SetObjectCountInChunk(size_t count) {
  cur_chunks_obj_count_ = count;
}

template<TypeID type_max>
template<typename T, typename ... Args>
Result<T*> ObjectPool<type_max>::CreateObject(Args&&... args) {
  static_assert(sizeof(T) >= sizeof(uint64_t)); // T must be at least one word long.
  static_assert(T::type_id || !T::type_id); // T should have a static type_id field.

  for (auto& chunk : lists_[T::type_id]) {
    if (!chunk.IsFull()) {
      auto res_ptr = chunk.template Add<T>(std::forward<Args>(args)...);
      if (res_ptr.IsOk()) {
        return res_ptr;
      }
    }
  }

  Chunk* new_chunk = new Chunk(sizeof(T), T::type_id, cur_chunks_obj_count_);

  lists_[T::type_id].PushFront(new_chunk);
  return lists_[T::type_id].begin()->template Add<T>(std::forward<Args>(args)...);
}

template<TypeID type_max>
template<typename T>
Status ObjectPool<type_max>::DeleteObject(T* obj_ptr) {
  for (auto& chunk : lists_[T::type_id]) {
    if (chunk.GetPresentStatus(obj_ptr).IsOk()) {
      return chunk.template Delete<T>(obj_ptr);
    }
  }

  return make_result::Fail(NOT_FOUND);
}

template<TypeID type_max>
ObjectPool<type_max>::~ObjectPool() {
  for (auto& chunks_ : lists_) {
    while (!chunks_.IsEmpty()) {
      Chunk* chunk = chunks_.PopFront();
      delete chunk;
    }
  }
}

template<TypeID type_max>
Status ObjectPool<type_max>::Free(void* data_ptr) {
  for (auto& chunks_ : lists_) {
    for (auto& chunk : chunks_) {
      if (chunk.GetPresentStatus(data_ptr).IsOk()) {
        chunk.HardDelete(data_ptr);
        return make_result::Ok();
      }
    }
  }

  return make_result::Fail(NOT_FOUND);
}


#endif // EFFECTIVE_BROCOLLI_OBJECT_POOL
