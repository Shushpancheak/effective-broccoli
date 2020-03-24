#ifndef EFFECTIVE_BROCOLLI_OBJECT_POOL
#define EFFECTIVE_BROCOLLI_OBJECT_POOL

#include <vector>

namespace objects {

template<size_t ObjectsPerChunk = 10>
class Pool {
public:

  /**
   * Create an object of type T with args.
   * type T must have a static type ID in field of type size_t
   * named type_id.
   */
  template<typename T, typename Args...>
  T* CreateObject(Args&&... args) {
    
  }


private:
  std::vector<Chunk<ObjectsPerChunk>> chunks_;
};

}

#endif // EFFECTIVE_BROCOLLI_OBJECT_POOL