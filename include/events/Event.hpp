#ifndef EFFECTIVE_BROCCOLI_EVENT
#define EFFECTIVE_BROCCOLI_EVENT

#include "support/typedefs.hpp"

enum {
  EVENT_NULL = 0,
  EVENT_TRANSFORM,

  // Transform events.
  EVENT_MOVE,
  EVENT_ROTATE,
  EVENT_SCALE,
  EVENT_SET_TRANSFORM,

  //Graphic events.
  EVENT_MOVE_GRAPHIC,
  EVENT_ROTATE_GRAPHIC,
  EVENT_SCALE_GRAPHIC,
  EVENT_SET_TRANSFORM_GRAPHIC,

  EVENT_COLLISION,
  EVENT_MAX
};

class Event {
public:
  // static const EventID type_id; -- add in children

  Event(const EventID event_id)
    : event_id(event_id) {};

  EventID event_id;
};

#endif