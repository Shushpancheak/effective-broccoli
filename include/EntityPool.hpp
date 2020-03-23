#ifndef EFFECTIVE_BROCOLLI_ENTITY_POOL
#define EFFECTIVE_BROCOLLI_ENTITY_POOL

#include <memory>
#include <vector>

#include "Entity.hpp"

class EntityPool {
public:


private:
  std::vector<Entity*> pool_;
};

#endif // EFFECTIVE_BROCOLLI_ENTITY_POOL