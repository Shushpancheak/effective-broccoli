#ifndef EFFECTIVE_BROCOLLI_SYSTEM_HPP
#define EFFECTIVE_BROCOLLI_SYSTEM_HPP

#include "support/TypeCounter.hpp"

template <typename T>
class System {
public:
  static const size_t type_id;

  static size_t GetTypeID()  {
    return type_id;
  }

  void operator delete(void*) = delete;
  void operator delete[](void*) = delete;
};

template <typename T>
const size_t System<T>::type_id = TypeCounter::GetId<T>();

#endif //EFFECTIVE_BROCOLLI_SYSTEM_HPP
