#ifndef EFFECTIVE_BROCOLLI_ISYSTEM_HPP
#define EFFECTIVE_BROCOLLI_ISYSTEM_HPP

#include "events/Event.hpp"
#include "support/typedefs.hpp"

using EventPtr = IEvent*;

class ISystem {
public:
  virtual ~ISystem() = 0;
  virtual void Accept(EventPtr event_ptr, EventID event_id) = 0;
};

#endif //EFFECTIVE_BROCOLLI_SYSTEM_HPP
