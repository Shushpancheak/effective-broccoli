#include "memory/DataChunk.hpp"

DataChunk::DataChunk(
  const size_t object_size,
  const TypeID type_id,
  const size_t object_count)
  : object_size_(object_size)
  , object_count_(object_count)
  , type_id_(type_id)
  , size_(0) {
  assert(object_size >= sizeof(uint64_t));

  // An extra +1 object for proper end(), which would point to the end of the buffer.
  buffer_start_ = new char[object_size * (object_count_ + 1)];
  buffer_end_ = buffer_start_ + (object_count_ + 1) * object_size_;
  Free(buffer_start_, object_count_ + 1);

  Construct<size_t>(buffer_end_, NON_EMPTY_MEMORY_BYTES).ThrowIfError();
}

DataChunk::~DataChunk() {
  delete[] buffer_start_;
}

void DataChunk::HardDelete(void* item_ptr) {
  Free(item_ptr, 1);

  --size_;
}

bool DataChunk::IsEmpty() const {
  return size_ == 0;
}

bool DataChunk::IsFull() const {
  return size_ == object_count_;
}

size_t DataChunk::Size() const {
  return size_;
}

DataChunk::TypeID DataChunk::GetTypeID() const {
  return type_id_;
}

// - - - - - - - - - - - - - - - - PRIVATES - - - - - - - - - - - - - - - - - -

void* DataChunk::GetAvailable(void* start, void* end) const {
  OP_ASSERT_IN_BUFFER_RANGE(start);
  OP_ASSERT_IN_BUFFER_RANGE_END_INCLUDED(end);
  OP_ASSERT_ADDRESS_ALIGNED(start);
  OP_ASSERT_ADDRESS_ALIGNED(end);
  assert(end >= start);

  for (char* addr = static_cast<char*>(start);
    addr != end;
    addr += object_size_) {
    if (IsAvailable(addr)) {
      return addr;
    }
  }

  return nullptr;
}

void* DataChunk::GetAvailable() const {
  // Starting from size_, because it is
  // the most likely spot to have a free space.
  void* buffer_at_size_ptr = buffer_start_ + size_ * object_size_;

  void* available_ptr = GetAvailable(buffer_at_size_ptr, buffer_end_);
  if (available_ptr) {
    return available_ptr;
  }

  // Now try to find somewhere from the start.
  return GetAvailable(buffer_start_, buffer_at_size_ptr);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void DataChunk::Free(void* item_addr, const size_t count) {
  OP_ASSERT_IN_BUFFER_RANGE(item_addr);
  OP_ASSERT_ADDRESS_ALIGNED(item_addr);

  char* item_byte_addr = static_cast<char*>(item_addr);
  char* end = item_byte_addr + count * object_size_;
  for (char* addr = item_byte_addr; addr < end; addr += object_size_) {
    *reinterpret_cast<size_t*>(addr) = EMPTY_MEMORY_BYTES;
  }
}

bool DataChunk::IsAvailable(void* item_addr) {
  return *static_cast<size_t*>(item_addr) == EMPTY_MEMORY_BYTES;
}
