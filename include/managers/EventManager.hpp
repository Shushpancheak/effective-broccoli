#ifndef EFFECTIVE_BROCCOLI_EVENT_MANAGER
#define EFFECTIVE_BROCCOLI_EVENT_MANAGER

#include <queue>
#include "support/result.hpp"
#include "events/Event.hpp"
#include "SystemManager.hpp"

class EventManager {
public:
  /**
   * Run event loop, but for a time that does
   * not exceed given duration (or maybe just slightly...).
   */
  void RunFor();

  /**
   *
   */
  Status Subscribe();

  Status Unsubscribe();

private:
  std::queue<Event*> events_queue_;
  SystemManager* sys_manager_ptr_;
};

#endif