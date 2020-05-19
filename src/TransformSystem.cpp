#include "systems/TransformSystem.hpp"

const SystemID TransformSystem::type_id;

TransformSystem::TransformSystem() {
  bro::GetEventManager()->Subscribe(
    TransformSystem::type_id, MoveEvent::type_id
  ).ThrowIfError();
  bro::GetEventManager()->Subscribe(
    TransformSystem::type_id, SetTransformEvent::type_id
  ).ThrowIfError();
}

void TransformSystem::Accept(EventPtr event_ptr) {
  switch(event_ptr->event_id) {
  case EVENT_MOVE: {
    auto* move_event_ptr = reinterpret_cast<MoveEvent*>(event_ptr);

    auto trans_comp =
      bro::GetComponent<TransformComponent>(move_event_ptr->entity_id);
    REPORT_IF_ERROR(trans_comp);

    trans_comp.ValueUnsafe()->rel_transform.translate(move_event_ptr->move_vec);

    const auto reg_res =
      bro::RegisterEvent<MoveGraphicEvent>(
        move_event_ptr->entity_id,
        move_event_ptr->move_vec
      );
    REPORT_IF_ERROR(reg_res);

    } break;

  case EVENT_SET_TRANSFORM: {
    auto* trans_event_ptr = reinterpret_cast<SetTransformEvent*>(event_ptr);

    auto trans_comp =
      bro::GetComponent<TransformComponent>(trans_event_ptr->entity_id);
    REPORT_IF_ERROR(trans_comp);

    trans_comp.ValueUnsafe()->rel_transform = trans_event_ptr->trans;

    const auto reg_res =
      bro::RegisterEvent<SetTransformGraphicEvent>(
        trans_event_ptr->entity_id, trans_event_ptr->trans
      );
    REPORT_IF_ERROR(reg_res);

    } break;

  default: break;

  }

  return;
}

void TransformSystem::Update(Duration delta_time) {
  
}
