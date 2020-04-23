#ifndef EFFECTIVE_BROCOLLI_SYSTEM_HPP
#define EFFECTIVE_BROCOLLI_SYSTEM_HPP

#include "events/Event.hpp"
#include "support/time.hpp"

enum {
  SYSTEM_NULL = 0,
  SYSTEM_GRAPHICAL,
  SYSTEM_PHYSICAL,
  SYSTEM_MAX
};

using EventPtr = Event*;
class System {
public:
  // static const TypeID type_id; -- add in children

  virtual ~System();
  virtual void Accept(EventPtr event_ptr) = 0;
          void UpdateBase();

  void operator delete(void*) = delete;
  void operator delete[](void*) = delete;

protected:
  virtual void Update(Duration delta_time) = 0;

private:
  TimeStamp last_update_time_;
  TimeStamp time_now_; // To not to create a new timestamp every update...
};

inline void System::UpdateBase() {
  time_now_ = Clock::now();
  this->Update(time_now_ - last_update_time_);
  last_update_time_ = time_now_;
}

#endif //EFFECTIVE_BROCOLLI_SYSTEM_HPP
