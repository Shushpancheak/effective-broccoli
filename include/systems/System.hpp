#ifndef EFFECTIVE_BROCOLLI_SYSTEM_HPP
#define EFFECTIVE_BROCOLLI_SYSTEM_HPP

#include "events/Event.hpp"


using EventPtr = Event*;
class System {
public:
  // static const TypeID type_id; -- add in children

  virtual ~System();
  virtual void Accept(EventPtr event_ptr) = 0;

  void operator delete(void*) = delete;
  void operator delete[](void*) = delete;
};

enum {
  SYSTEM_GRAPHICAL = 0,
  SYSTEM_PHYSICAL,
  SYSTEM_MAX
};

#endif //EFFECTIVE_BROCOLLI_SYSTEM_HPP
