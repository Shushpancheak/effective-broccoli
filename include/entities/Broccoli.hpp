#ifndef EFFECTIVE_BROCCOLI_BROCCOLI_HPP
#define EFFECTIVE_BROCCOLI_BROCCOLI_HPP

#include "Entity.hpp"

#include "engine/core.hpp"
#include "components/TransformComponent.hpp"

class BroccoliEntity : public Entity {
public:
  static const EntityID type_id = ENTITY_BROCCOLI;

  int32_t padding;

  BroccoliEntity(const EntityID entity_id)
    : Entity(entity_id) {
    REPORT_IF_ERROR(
      bro::GetComponentManager()->
        AddComponent<TransformComponent>(entity_id, nullptr));

    GraphicalSystem* graphical_sys =
      dynamic_cast<GraphicalSystem*>(
        bro::GetSystemManager()->GetSystem(GraphicalSystem::type_id).ValueUnsafe()
      );

    REPORT_IF_ERROR(
      bro::GetComponentManager()->
        AddComponent<GraphicalComponent>(entity_id, graphical_sys->GetMiddle()));

    auto graphical_component =
      bro::GetComponentManager()->
        GetComponent<GraphicalComponent>(entity_id);

    graphical_component.ThrowIfError();

    graphical_component.ValueUnsafe()->sprite.setTexture(
      bro::GetResourceManager()->Get("img/effective-broccoli.png")
    );
  }

private:

};

#endif // EFFECTIVE_BROCCOLI_BROCCOLI_HPP
