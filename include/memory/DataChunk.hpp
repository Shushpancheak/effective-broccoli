#ifndef EFFECTIVE_BROCOLLI_DATA_CHUNK
#define EFFECTIVE_BROCOLLI_DATA_CHUNK

#include <utility>
#include <cassert>
#include "support/IntrusiveList.hpp"
#include "constants/error.hpp"
#include "support/result.hpp"

/**
 * Chunk of objects of certain type, that is decided at runtime.
 * Use Add and Delete to manage objects.
 *
 * The type of items stored must have a static type ID
 * given in static field type_id.
 *
 * The type stored also has to be at least one word long.
 */
class DataChunk : public IntrusiveNode<DataChunk> {
  using TypeID = size_t;

public:
  explicit DataChunk(size_t object_size, TypeID type_id,
                     size_t object_count = 100);
  ~DataChunk();

  DataChunk(const DataChunk& other) = delete;
  DataChunk(DataChunk&& other) = delete;
  DataChunk& operator=(const DataChunk& other) = delete;
  DataChunk& operator=(DataChunk&& other) = delete;

  /**
   * Emplaces the object with given args.
   *
   * @return Address of constructed object;
   * IS_FULL - chunk is already full.
   */
  template<typename T, typename... Args>
  Result<T*> Add(Args&&... args);

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
  Status Delete(T* item_ptr);

  /**
   * Deletes count objects of type T, starting from item_ptr, using T's destructor.
   */
  template<typename T>
  Status Delete(T* item_ptr, size_t count);

  /**
   * Deletes the object pointed to by item_ptr, using T's destructor,
   * if an object does actually occupy this position.
   *
   * @return ALREADY_DELETED if the address is not occupied.
   */
  template<typename T>
  Status DeleteIfPresent(T* item_ptr);

  /**
   * Deletes objects starting from item_ptr, using T's destructor,
   * if an object does actually occupy this position.
   *
   * @return ALREADY_DELETED if the address is not occupied.
   */
  template<typename T>
  Status DeleteIfPresent(T* item_ptr, size_t count);

  /**
   * Deletes all objects in DataChunk, skipping uninitialized components,
   * thus clearing all DataChunk.
   */
  template<typename T>
  void DeleteAll();

  /**
   * @return NO_ERROR if item pointed to by item_ptr is present in DataChunk.
   * Error codes:
   * OUT_OF_BOUNDS      - Pointer is out of bounds of DataChunk.
   * OBJECT_NOT_PRESENT - There is no object in address.
   */
  template<typename T>
  Status GetPresentStatus(T* item_ptr);

  /**
   * @return does DataChunk has no objects.
   */
  [[nodiscard]] bool IsEmpty() const;

  /**
   * @return is DataChunk full with objects.
   */
  [[nodiscard]] bool IsFull() const;

  /**
   * @return number of objects held in DataChunk.
   */
  [[nodiscard]] size_t Size() const;

  /**
   * @return static type id of object type held in DataChunk.
   */
  [[nodiscard]] TypeID GetTypeID() const;

private:
  /**
   * Construct the item of type T with args.
   *
   * @return error codes:
   * FALSE_TYPE  - type T does not correspond to the static type_id given at Chunk's creation.
   * CTOR_FAILED - constructor of type T with args failed.
   * BAD_PTR     - item_addr is nullptr.
   */
  template<typename T, typename... Args>
  Status Construct(void* item_addr, Args&&... args);

  /**
   * Free items starting from `item_addr` up to (T*)item_addr + count
   */
  void Free(void* item_addr, size_t count);

  /**
   * Check if the item pointed to by `item_addr` is freed.
   */
  static bool IsAvailable(void* item_addr);

  /**
   * Finds first available address in given range [start, end).
   */
  void* GetAvailable(void* start, void* end) const;

  /**
   * Get first available address where a new object can be placed.
   *
   * @return Address of the item; nullptr on failure.
   */
  [[nodiscard]] void* GetAvailable() const;

// Utilities
private:
  #define OP_ASSERT_IN_BUFFER_RANGE(ptr) \
    assert(reinterpret_cast<char*>(ptr) >= buffer_start_ && \
           reinterpret_cast<char*>(ptr) <  buffer_end_)
  #define OP_ASSERT_IN_BUFFER_RANGE_END_INCLUDED(ptr) \
    assert(reinterpret_cast<char*>(ptr) >= buffer_start_ && \
           reinterpret_cast<char*>(ptr) <= buffer_end_)
  #define OP_ASSERT_ADDRESS_ALIGNED(ptr) \
    assert((reinterpret_cast<size_t>(ptr) - \
            reinterpret_cast<size_t>(buffer_start_)) % object_size_ == 0)

// Fields
private:
  static const size_t EMPTY_MEMORY_BYTES = 0xDEADBEEF;
  static const size_t NON_EMPTY_MEMORY_BYTES = 0xBEEFDEAD; // must not be equal to empty bytes

  char* buffer_start_;
  char* buffer_end_; // Points to memory just outside the buffer.
  const size_t object_size_;
  const size_t object_count_;
  const TypeID type_id_;

  // Not really needed, but it can greatly optimize IsEmpty().
  size_t size_;

// Iterators
public:
  template<typename T>
  class Iterator {
  public:
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;
    using iterator_category = std::forward_iterator_tag;

    explicit Iterator(T* ptr)
      : ptr_(ptr) {
      while (DataChunk::IsAvailable(ptr_)) {
        ++ptr_;
      }
    }

    Iterator& operator++() {
      do {
        ++ptr_;
      } while (DataChunk::IsAvailable(ptr_));
      return *this;
    }

    Iterator operator++(int) {
      Iterator res = *this;
      ++(*this);
      return res;
    }

    bool operator==(const Iterator& other) const {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const Iterator& other) const {
      return ptr_ != other.ptr_;
    }

    T operator*() {
      return *ptr_;
    }

  private:
    T* ptr_;
  };

  template<typename T>
  Iterator<T> begin() {
    return Iterator<T>(buffer_start_);
  }

  
  template<typename T>
  Iterator<T> end() {
    return Iterator<T>(buffer_end_);
  }
};

template<typename T, typename ... Args>
Result<T*> DataChunk::Add(Args&&... args) {
  if (this->IsFull()) {
    return make_result::Fail(IS_FULL);
  }

  void* available_ptr = GetAvailable();
  assert(available_ptr != nullptr); // Checked for fullness earlier.
  OP_ASSERT_IN_BUFFER_RANGE(available_ptr);

  const auto res = Construct<T, Args...>(available_ptr, std::forward<Args>(args)...);

  if (!res.HasError()) {
    ++size_;
    return make_result::Ok(static_cast<T*>(available_ptr));
  }

  return make_result::PropagateError(res);
}

template<typename T>
Status DataChunk::Delete(T* item_ptr) {
  assert(!IsEmpty());
  OP_ASSERT_IN_BUFFER_RANGE(item_ptr);

  item_ptr->~T();
  Free(item_ptr, 1);

  --size_;

  return make_result::Ok();
}

template<typename T>
void DataChunk::DeleteAll() {
  for (size_t i = 0; i < object_count_; ++i) {
    auto res = DeleteIfPresent(buffer_start_ + i);
  }
}

template<typename T>
Status DataChunk::GetPresentStatus(T* item_ptr) {
  if (item_ptr < buffer_start_ || item_ptr > buffer_start_ + object_count_ * object_size_) {
    return make_result::Fail(OUT_OF_BOUNDS);
  }

  if (IsAvailable(item_ptr)) {
    return make_result::Fail(OBJECT_NOT_PRESENT);
  }
  else {
    return make_result::Ok();
  }
}

template<typename T>
Status DataChunk::Delete(T* item_ptr, size_t count) {
  for (size_t i = 0; i < count; ++i) {
    auto res = Delete(item_ptr + i);

    if (res.HasError()) {
      return res;
    }
  }

  return make_result::Ok();
}

template<typename T>
Status DataChunk::DeleteIfPresent(T* item_ptr) {
  if (IsAvailable(item_ptr)) {
    return make_result::Fail(ALREADY_DELETED);
  }

  return Delete(item_ptr);
}

template<typename T>
Status DataChunk::DeleteIfPresent(T* item_ptr, size_t count) {
  for (size_t i = 0; i < count; ++i) {
    auto res = DeleteIfPresent(item_ptr + i);

    if (res.HasError()) {
      return res;
    }
  }

  return make_result::Ok();
}

template<typename T, typename ... Args>
Status DataChunk::Construct(void* item_addr, Args&&... args) {
  OP_ASSERT_IN_BUFFER_RANGE(item_addr);
  OP_ASSERT_ADDRESS_ALIGNED(item_addr);

  if (T::type_id != type_id_) {
    return make_result::Fail(FALSE_TYPE);
  }

  if (item_addr == nullptr) {
    return make_result::Fail(BAD_PTR);
  }

  void* res_ptr = new (item_addr) T(std::forward<Args>(args)...);

  return (res_ptr == nullptr ? make_result::Fail(CTOR_FAILED) : make_result::Ok());
}

#endif