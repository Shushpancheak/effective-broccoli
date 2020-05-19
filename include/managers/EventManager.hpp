#ifndef EFFECTIVE_BROCCOLI_EVENT_MANAGER
#define EFFECTIVE_BROCCOLI_EVENT_MANAGER

#include <queue>
#include <unordered_map>
#include "support/result.hpp"
#include "support/time.hpp"
#include "events/Event.hpp"
#include "memory/ObjectPool.hpp"
#include "managers/SystemManager.hpp"

class EventManager {
  using EventPtr = Event*;

public:
  explicit EventManager(SystemManager* system_man);

  /**
   * Run event loop, but for a time that does
   * not exceed given duration (or maybe just slightly...).
   */
  void RunFor(Duration duration);

  /**
   * Put an event on the event queue.
   */
  template<typename T, typename... Args>
  Status RegisterEvent(Args&&... args);

  /**
   * Acquire a subscription to a certain type of event,
   * determined by its ID. Once an event of that type is
   * popped from the queue, a reference to the object of event will be
   * passed to Accept method of a System.
   */
  Status Subscribe(SystemID sys_id, EventID event_id);

  /**
   * Stop receiving events.
   */
  Status Unsubscribe(SystemID sys_id, EventID event_id);

private:
  ObjectPool<EVENT_MAX> events_pool_;
  std::queue<EventPtr> events_queue_{};
  std::unordered_multimap<EventID, SystemID> subscribed_systems_map_{};
  SystemManager* system_man_ptr;
};

template<typename T, typename ...Args>
Status EventManager::RegisterEvent(Args&&... args) {
  auto create_res = events_pool_.CreateObject<T>(std::forward<Args>(args)...);

  CHECK_ERROR(create_res);

  events_queue_.push(static_cast<EventPtr>(create_res.ValueUnsafe()));

  return make_result::Ok();
}

#endif
