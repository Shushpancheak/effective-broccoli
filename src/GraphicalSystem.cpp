#include "systems/GraphicalSystem.hpp"

#define NEW_GRAPHICAL_ENTITY bro::GetEntityManager()->AddEntity<GraphicalPlaceholderEntity>()

GraphicalSystem::GraphicalSystem()
  : root_      (NEW_GRAPHICAL_ENTITY, nullptr)
  , background_(NEW_GRAPHICAL_ENTITY, &root_)
  , middle_    (NEW_GRAPHICAL_ENTITY, &root_)
  , foreground_(NEW_GRAPHICAL_ENTITY, &root_) {
  bro::GetEventManager()->Subscribe(
    GraphicalSystem::type_id, MoveGraphicEvent::type_id).ThrowIfError();
}

void GraphicalSystem::Accept(EventPtr event_ptr) {
  if (event_ptr->event_id == EVENT_MOVE_GRAPHIC) {
    auto* move_event_ptr = reinterpret_cast<MoveGraphicEvent*>(event_ptr);

    auto graphical_comp =
      bro::GetComponentManager()->
        GetComponent<GraphicalComponent>(move_event_ptr->entity_id);

    REPORT_IF_ERROR(graphical_comp);

    graphical_comp.ValueUnsafe()->rel_transform.translate(move_event_ptr->move_vec);
  }

  return;
}

void GraphicalSystem::Update(Duration delta_time) {
  UpdateRecursive(&root_, nullptr);
}

void GraphicalSystem::UpdateRecursive(GraphicalComponent* node, GraphicalComponent* prev) {
  for (auto& next_component : node->children) {
    UpdateRecursive(next_component, node);
  }

  if (prev != nullptr) {
    auto prev_trans = prev->sprite.getTransform();
    node->abs_transform = prev_trans.combine(node->rel_transform);
  }

  bro::GetWindow()->draw(node->sprite, node->abs_transform);
}
