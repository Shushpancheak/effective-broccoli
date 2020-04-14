#ifndef EFFECTIVE_BROCCOLI_IEVENT
#define EFFECTIVE_BROCCOLI_IEVENT

#include "support/TypeCounter.hpp"
#include "support/typedefs.hpp"

class IEvent {
public:
  explicit IEvent(const EventID event_id) : event_id(event_id) {};
  virtual ~IEvent() = 0;

  const EventID event_id;
};

#endif