#include "managers/EventManager.hpp"

EventManager::EventManager(SystemManager* sys_manager_ptr)
  : events_pool_()
  , events_queue_()
  , subscribed_systems_map_()
  , sys_manager_ptr_(sys_manager_ptr) {}

void EventManager::RunFor(Duration duration) {
  const StopWatch watch;

  while (!events_queue_.empty() && watch.Elapsed() < duration) {
    const auto event_ptr = events_queue_.front();
    events_queue_.pop();

    const EventID event_id = event_ptr->event_id;
    auto iter_pair = subscribed_systems_map_.equal_range(event_id);
    auto iter = iter_pair.first;
    auto end = iter_pair.second;
    for (; iter != end; ++iter) {
      auto system = sys_manager_ptr_->GetSystem(iter->second);

      assert(system.IsOk());

      system.ValueUnsafe()->Accept(event_ptr, event_id);
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
