#ifndef EFFECTIVE_BROCCOLI_GRAPHIC_EVENTS_HPP
#define EFFECTIVE_BROCCOLI_GRAPHIC_EVENTS_HPP

#include <utility>

#include "Event.hpp"
#include "SFML/System/Vector2.hpp"

struct MoveGraphicEvent : public Event {
  static const EventID type_id = EVENT_MOVE_GRAPHIC;

  MoveGraphicEvent(EntityID entity_id, const sf::Vector2f vec)
    : Event(type_id)
    , entity_id(entity_id)
    , move_vec(vec) {}

  EntityID entity_id;
  sf::Vector2f move_vec;
};

#endif
