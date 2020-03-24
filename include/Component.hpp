#ifndef EFFECTIVE_BROCOLLI_COMPONENT_HPP
#define EFFECTIVE_BROCOLLI_COMPONENT_HPP

#include "TypeCounter.hpp"

template <class T>
class Component {
public:
  static const size_t type_id;

  static size_t GetTypeID()  {
    return type_id;
  }

  size_t GetOwner() const {
    return entity_id_;
  }

protected:
  size_t entity_id_ = 0;
};

template <typename T>
const size_t Component<T>::type_id = TypeCounter::GetId<T>();
#endif //EFFECTIVE_BROCOLLI_COMPONENT_HPP
