#ifndef EFFECTIVE_BROCCOLI_BROCCOLI_HPP
#define EFFECTIVE_BROCCOLI_BROCCOLI_HPP

#include "Entity.hpp"

#include "engine/core.hpp"
#include "components/TransformComponent.hpp"
#include "components/PhysicalComponent.hpp"

class BroccoliEntity : public Entity {
public:
  static const EntityID type_id = ENTITY_BROCCOLI;

  int32_t padding;

  BroccoliEntity(const EntityID entity_id)
    : Entity(entity_id) {
    REPORT_IF_ERROR(
      bro::GetComponentManager()->
        AddComponent<TransformComponent>(entity_id, nullptr));

    REPORT_IF_ERROR(
      bro::GetComponentManager()->
        AddComponent<PhysicalComponent>(
          entity_id, sf::Rect<float>({0, 0, 10, 10}), 1, PhysicalGroup::DynamicObject)
        );
    bro::GetSystem<PhysicalSystem>()->AddMovingEntity(entity_id);
    bro::GetComponentManager()->
      GetComponent<PhysicalComponent>(entity_id).ValueUnsafe()->force_ = {1, 1};

    REPORT_IF_ERROR(
      bro::GetComponentManager()->
        AddComponent<GraphicalComponent>(
          entity_id,
          bro::GetSystem<GraphicalSystem>()->GetMiddle()
        )
    );

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
