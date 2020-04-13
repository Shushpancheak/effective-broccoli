#include "memory/ObjectPool.hpp"

ObjectPool::~ObjectPool() {
  while (!chunks_.IsEmpty()) {
    Chunk* chunk = chunks_.PopFront();
    delete chunk;
  }
}

Status ObjectPool::Free(void* data_ptr) {
  for (auto& chunk : chunks_) {
    if (chunk.GetPresentStatus(data_ptr).IsOk()) {
      chunk.HardDelete(data_ptr);
      return make_result::Ok();
    }
  }

  return make_result::Fail(NOT_FOUND);
}
