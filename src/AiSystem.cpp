#include "systems/AiSystem.hpp"
#include <math.h>
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
  double dt = DurationToDouble(delta_time);

  auto* comp_man = bro::GetComponentManager();
  for (auto component_it =
      comp_man->GetIteratorBegin<AiComponent>();
       component_it != comp_man->GetIteratorEnd<AiComponent>();
       ++component_it) {
      auto player_hitbox = bro::GetComponentManager()->GetComponent<PhysicalComponent>(component_it->target_id).Value()->hitbox_;
      auto mob_hitbox = bro::GetComponentManager()->GetComponent<PhysicalComponent>(component_it->entity_id).Value()->hitbox_;

      const double len = 130.0;
      double k = std::abs((player_hitbox.top - mob_hitbox.top) / (player_hitbox.left - mob_hitbox.left));
      double new_vel_x = std::sqrt((len * len) / (k * k + 1));
      double new_vel_y = new_vel_x * k;
      int x_sign = player_hitbox.left - mob_hitbox.left > 0? 1 : -1;
      int y_sign = player_hitbox.top - mob_hitbox.top > 0? 1 : -1;
      new_vel_y *= y_sign;
      if (std::abs(player_hitbox.left - mob_hitbox.left) < 1e-7 &&
          std::abs(player_hitbox.top - mob_hitbox.top) < 1e-7) {
        new_vel_y = len * y_sign;
        new_vel_x = 0;
      }
      new_vel_x *= x_sign;
//      bro::RegisterEvent<ChangeSpeedEvent>(component_it->entity_id, {static_cast<float>(new_vel_x), static_cast<float>(new_vel_y)});
      bro::GetComponentManager()->GetComponent<PhysicalComponent>(component_it->entity_id).Value()->velocity_ = {static_cast<float>(new_vel_x), static_cast<float>(new_vel_y)};
  }
}
