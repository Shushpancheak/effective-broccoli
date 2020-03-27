#ifndef EFFECTIVE_BROCOLLI_DATA_CHUNK
#define EFFECTIVE_BROCOLLI_DATA_CHUNK

#include <utility>
#include <cassert>
#include "support/IntrusiveList.hpp"
#include "constants/error_codes.hpp"

/**
 * Chunk of objects of certain type, that is decided at runtime.
 * Use Add and Delete to manage objects.
 *
 * The type of items stored must have a static type ID
 * given in static method GetTypeID().
 *
 * The type stored also has to be at least one word long.
 */
class DataChunk : public IntrusiveNode<DataChunk> {
  using TypeID = size_t;

public:
  explicit DataChunk(const size_t object_size, const TypeID type_id,
                 const size_t object_count = 100);
  ~DataChunk();

  DataChunk(const DataChunk& other) = delete;
  DataChunk(DataChunk&& other) = delete;
  DataChunk& operator=(const DataChunk& other) = delete;
  DataChunk& operator=(DataChunk&& other) = delete;

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
  int GetPresentStatus(T* item_ptr);

  /**
   * @return does DataChunk has no objects.
   */
  bool IsEmpty() const;

  /**
   * @return is DataChunk full with objects.
   */
  bool IsFull() const;

  /**
   * @return number of objects held in DataChunk.
   */
  size_t Size() const;

  /**
   * @return static type id of object type held in DataChunk.
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
   * Finds first available address in given range [start, end).
   */
  void* GetAvailable(void* start, void* end) const;

  /**
   * Get first available address where a new object can be placed.
   *
   * @return Address of the item; nullptr on failure.
   */
  void* GetAvailable() const;

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

  char* buffer_start_;
  char* buffer_end_; // Points to memory just outside the buffer.
  const size_t object_size_;
  const size_t object_count_;
  const TypeID type_id_;

  // Not really needed, but it can greatly optimize IsEmpty().
  size_t size_;
};

template<typename T, typename ... Args>
T* DataChunk::Add(Args&&... args) {
  if (this->IsFull()) {
    return nullptr;
  }

  void* available_ptr = GetAvailable();
  assert(available_ptr != nullptr); // Checked for fullness earlier.
  OP_ASSERT_IN_BUFFER_RANGE(available_ptr);

  const int res = Construct<T, Args...>(available_ptr, std::forward<Args>(args)...);

  if (res == 0) {
    ++size_;
    return reinterpret_cast<T*>(available_ptr);
  }

  return nullptr;
}

template<typename T>
int DataChunk::Delete(T* item_ptr) {
  assert(!IsEmpty());
  OP_ASSERT_IN_BUFFER_RANGE(item_ptr);

  item_ptr->~T();
  Free(item_ptr, 1);

  --size_;

  return NO_ERROR;
}

template<typename T>
void DataChunk::DeleteAll() {
  for (size_t i = 0; i < object_count_; ++i) {
    DeleteIfPresent(buffer_start_ + i);
  }
}

template<typename T>
int DataChunk::GetPresentStatus(T* item_ptr) {
  if (item_ptr < buffer_start_ || item_ptr > buffer_start_ + object_count_ * object_size_) {
    return OUT_OF_BOUNDS;
  }

  if (IsAvailable(item_ptr)) {
    return OBJECT_NOT_PRESENT;
  }
  else {
    return NO_ERROR;
  }
}

template<typename T>
int DataChunk::Delete(T* item_ptr, size_t count) {
  for (size_t i = 0; i < count; ++i) {
    const int res = Delete(item_ptr + i);

    if (res != NO_ERROR) {
      return res;
    }
  }

  return NO_ERROR;
}

template<typename T>
int DataChunk::DeleteIfPresent(T* item_ptr) {
  if (IsAvailable(item_ptr)) {
    return ALREADY_DELETED;
  }

  return Delete(item_ptr);
}

template<typename T>
int DataChunk::DeleteIfPresent(T* item_ptr, size_t count) {
  for (size_t i = 0; i < count; ++i) {
    const int res = DeleteIfPresent(item_ptr + i);

    if (res != NO_ERROR) {
      return res;
    }
  }

  return NO_ERROR;
}

template<typename T, typename ... Args>
int DataChunk::Construct(void* item_addr, Args&&... args) {
  OP_ASSERT_IN_BUFFER_RANGE(item_addr);
  OP_ASSERT_ADDRESS_ALIGNED(item_addr);

  if (T::GetTypeID() != type_id_) {
    return FALSE_TYPE;
  }

  if (item_addr == nullptr) {
    return BAD_PTR;
  }

  void* res_ptr = new (item_addr) T(std::forward<Args>(args)...);

  return (res_ptr == nullptr ? CTOR_FAILED : 0);
}

#endif