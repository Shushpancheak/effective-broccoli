#ifndef EFFECTIVE_BROCCOLI_PHYSICAL_EVENTS_HPP
#define EFFECTIVE_BROCCOLI_PHYSICAL_EVENTS_HPP

#include <utility>

#include "Event.hpp"
#include "SFML/System/Vector2.hpp"

struct CollisionEvent : public Event {
  static const EventID type_id = EVENT_COLLISION;

  CollisionEvent(const EntityID entity_id_1, const EntityID entity_id_2)
    : Event(type_id)
    , entity_id_1(entity_id_1)
    , entity_id_2(entity_id_2) {}

  EntityID entity_id_1;
  EntityID entity_id_2;
};

#endif
