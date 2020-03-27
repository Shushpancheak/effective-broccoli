#include "memory/ObjectPool.hpp"

ObjectPool::~ObjectPool() {
  while (!chunks_.IsEmpty()) {
    Chunk* chunk = chunks_.PopFront();
    delete chunk;
  }
}

int ObjectPool::Free(void* data_ptr) {
  for (auto& chunk : chunks_) {
    if (chunk.GetPresentStatus(data_ptr) == NO_ERROR) {
      chunk.HardDelete(data_ptr);
      return NO_ERROR;
    }
  }

  return NOT_FOUND;
}
