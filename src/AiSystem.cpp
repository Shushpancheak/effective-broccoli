#include "systems/AiSystem.hpp"

const SystemID AiSystem::type_id;

AiSystem::AiSystem() {
  bro::GetEventManager()->Subscribe(
    AiSystem::type_id, CollisionEvent::type_id
  ).ThrowIfError();
}

void AiSystem::Accept(EventPtr event_ptr) {
  switch(event_ptr->event_id) {
  case EVENT_COLLISION: {
    auto* coll_event = reinterpret_cast<CollisionEvent*>(event_ptr);

    EntityID aggressive_id = 0;
    auto ai_comp_1 = bro::GetComponent<AiComponent>(coll_event->entity_id_1);
    auto ai_comp_2 = bro::GetComponent<AiComponent>(coll_event->entity_id_2);
    if (ai_comp_1.IsOk() && ai_comp_1.ValueUnsafe()->type == AiType::AGGRESSIVE) {
      aggressive_id = coll_event->entity_id_1;
    }
    if (ai_comp_2.IsOk() && ai_comp_2.ValueUnsafe()->type == AiType::AGGRESSIVE) {
      aggressive_id = coll_event->entity_id_2;
    }

    if (aggressive_id == 0) {
      return;
    }

    // LOGIC FOR AGGRESSIVE AI.

    auto graph_comp = bro::GetComponent<GraphicalComponent>(aggressive_id);

    graph_comp.ThrowIfError();

    auto& sprite = graph_comp.ValueUnsafe()->sprite;

    sprite.setScale(sprite.getScale() + sf::Vector2f{0.1, 0.1});
    } break;

  default: break;

  }

  return;
}

void AiSystem::Update(Duration delta_time) {
  
}
