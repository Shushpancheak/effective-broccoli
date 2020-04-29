#include "systems/TransformSystem.hpp"

TransformSystem::TransformSystem() {
  bro::GetEventManager()->Subscribe(
    TransformSystem::type_id, MoveEvent::type_id).ThrowIfError();
}

void TransformSystem::Accept(EventPtr event_ptr) {
  if (event_ptr->event_id == EVENT_MOVE) {
    auto* move_event_ptr = reinterpret_cast<MoveEvent*>(event_ptr);

    auto trans_comp =
      bro::GetComponentManager()->
        GetComponent<TransformComponent>(move_event_ptr->entity_id);

    REPORT_IF_ERROR(trans_comp);

    trans_comp.ValueUnsafe()->rel_transform.translate(move_event_ptr->move_vec);

    const auto reg_res = bro::GetEventManager()->
      RegisterEvent<MoveGraphicEvent>(move_event_ptr->move_vec);

    REPORT_IF_ERROR(reg_res);
  }

  return;
}

void TransformSystem::Update(Duration delta_time) {
  
}
