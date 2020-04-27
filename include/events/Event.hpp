#ifndef EFFECTIVE_BROCCOLI_EVENT
#define EFFECTIVE_BROCCOLI_EVENT

#include "support/TypeCounter.hpp"
#include "support/typedefs.hpp"
#include "IEvent.hpp"

template <typename T>
class Event : public IEvent {
public:
  Event();

  static const TypeID type_id;

  static TypeID GetTypeID() {
    return type_id;
  }

};

template<typename T>
Event<T>::Event()
  : IEvent(type_id) {}

template <typename T>
const TypeID Event<T>::type_id = TypeCounter::GetId<T>();

#endif