#ifndef EFFECTIVE_BROCCOLI_EVENT
#define EFFECTIVE_BROCCOLI_EVENT

#include "support/typedefs.hpp"

class Event {
public:
  // static const EventID type_id; -- add in children

  Event(const EventID event_id)
    : event_id_(event_id) {};

  EventID GetEventID() const { return event_id_; }

  void operator delete(void*) = delete;
  void operator delete[](void*) = delete;

private:
  EventID event_id_;
};

enum {
  EVENT_COLLISION = 0,
  EVENT_MAX
};

#endif