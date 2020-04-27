//
// Created by dm on 4/15/20.
//

#ifndef EFFECTIVE_BROCOLLI_INCLUDE_EVENTS_MOVEALLEVENT_HPP_
#define EFFECTIVE_BROCOLLI_INCLUDE_EVENTS_MOVEALLEVENT_HPP_
#include "Event.hpp"
#include "support/time.hpp"

class MoveAllEvent : public Event<MoveAllEvent> {
 public:
  TimeStamp previous_move_timepoint;
  MoveAllEvent() : previous_move_timepoint(Clock::now()) {}
};

#endif //EFFECTIVE_BROCOLLI_INCLUDE_EVENTS_MOVEALLEVENT_HPP_
