#include <events/view_events.hpp>
#include "systems/ViewSystem.hpp"

const SystemID ViewSystem::type_id;

ViewSystem::ViewSystem(sf::RenderWindow* window) : view(sf::FloatRect(-1000, -1000, 5000, 5000)) {
  bro::GetEventManager()->Subscribe(
      ViewSystem::type_id, MoveViewEvent::type_id
  ).ThrowIfError();
  bro::GetEventManager()->Subscribe(
      ViewSystem::type_id, RotateViewEvent::type_id
  ).ThrowIfError();
  window->setView(view);
}

void ViewSystem::Accept(EventPtr event_ptr) {
  switch (event_ptr->event_id) {
    case EVENT_MOVE_VIEW: {
      auto *move_event_ptr = reinterpret_cast<MoveViewEvent *>(event_ptr);
      view.move(move_event_ptr->move_vec);
      break;
    }
    case EVENT_ROTATE_VIEW: {
      auto *rotate_event_ptr = reinterpret_cast<RotateViewEvent *>(event_ptr);
      view.rotate(rotate_event_ptr->degree);
      break;
    }
    default:
      break;
  }
  bro::GetWindow()->setView(view);
}

void ViewSystem::Update(Duration delta_time) {
}
