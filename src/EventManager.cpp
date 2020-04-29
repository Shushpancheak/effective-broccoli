#include "managers/EventManager.hpp"

EventManager::EventManager(SystemManager* system_man)
  : events_pool_()
  , events_queue_()
  , subscribed_systems_map_()
  , system_man_ptr(system_man) {}

void EventManager::RunFor(const Duration duration) {
  const StopWatch watch;

  while (!events_queue_.empty() && watch.Elapsed() < duration) {
    auto* const event_ptr = events_queue_.front();
    events_queue_.pop();

    const EventID event_id = event_ptr->event_id;
    const auto iter_pair = subscribed_systems_map_.equal_range(event_id);
    auto iter = iter_pair.first;
    auto end = iter_pair.second;
    for (; iter != end; ++iter) {
      auto system = system_man_ptr->GetSystem(iter->second);

      system.ThrowIfError();

      system.ValueUnsafe()->Accept(event_ptr);
    }

    auto res = events_pool_.Free(static_cast<void*>(event_ptr));
  }
}

Status EventManager::Subscribe(SystemID sys_id, EventID event_id) {
  subscribed_systems_map_.emplace(sys_id, event_id);
  return make_result::Ok();
}

Status EventManager::Unsubscribe(SystemID sys_id, EventID event_id) {
  const auto iter_pair = subscribed_systems_map_.equal_range(event_id);
  auto iter = iter_pair.first;
  const auto end = iter_pair.second;
  for (; iter != end; ++iter) {
    if (iter->second == sys_id) {
      subscribed_systems_map_.erase(iter);
      return make_result::Ok();
    }
  }

  return make_result::Fail(SUBSCRIPTION_NOT_FOUND);
}
