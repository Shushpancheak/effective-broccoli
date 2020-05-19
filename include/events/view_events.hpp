#ifndef EFFECTIVE_BROCOLLI_VIEW_EVENTS_HPP
#define EFFECTIVE_BROCOLLI_VIEW_EVENTS_HPP

#include <utility>

#include "Event.hpp"
#include "SFML/System/Vector2.hpp"

struct MoveViewEvent : public Event {
  static const EventID type_id = EVENT_MOVE_VIEW;

  explicit MoveViewEvent(const sf::Vector2f vec)
      : Event(type_id)
      , move_vec(vec) {}

  sf::Vector2f move_vec;
};

struct RotateViewEvent : public Event {
  static const EventID type_id = EVENT_ROTATE_VIEW;

  explicit RotateViewEvent(float deg)
      : Event(type_id)
      , degree(deg) {}

  float degree;
};

#endif //EFFECTIVE_BROCOLLI_VIEW_EVENTS_HPP
