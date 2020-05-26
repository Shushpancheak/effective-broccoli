#ifndef EFFECTIVE_BROCCOLI_EVENT
#define EFFECTIVE_BROCCOLI_EVENT

#include "support/typedefs.hpp"
#include "support/time.hpp"

enum {
  EVENT_NULL = 0,
  EVENT_TRANSFORM,

  // Transform events.
  EVENT_MOVE,
  EVENT_ROTATE,
  EVENT_SCALE,
  EVENT_SET_TRANSFORM,

  // Graphic events.
  EVENT_MOVE_GRAPHIC,
  EVENT_ROTATE_GRAPHIC,
  EVENT_SCALE_GRAPHIC,
  EVENT_SET_TRANSFORM_GRAPHIC,

  // View events.
  EVENT_MOVE_VIEW,
  EVENT_ROTATE_VIEW,

  EVENT_COLLISION,
  EVENT_CHANGE_SPEED,
  EVENT_MAX
};

class Event {
public:
  static const EventID type_id = EVENT_NULL; // -- add in children

  explicit Event(const EventID event_id)
    : event_id(event_id)
    , time_created(Clock::now()) {}

  EventID event_id;
  TimeStamp time_created;
};

#endif