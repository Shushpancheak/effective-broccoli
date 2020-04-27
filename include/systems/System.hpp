#ifndef EFFECTIVE_BROCOLLI_SYSTEM_HPP
#define EFFECTIVE_BROCOLLI_SYSTEM_HPP

#include "support/TypeCounter.hpp"
#include "systems/ISystem.hpp"

template <typename T>
class System : public ISystem {
public:
  static const TypeID type_id;

  static TypeID GetTypeID()  {
    return type_id;
  }

};

template <typename T>
const TypeID System<T>::type_id = TypeCounter::GetId<T>();

#endif //EFFECTIVE_BROCOLLI_SYSTEM_HPP
