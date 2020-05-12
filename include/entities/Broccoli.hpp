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
      bro::AddComponent<TransformComponent>(entity_id, nullptr)
    );
    REPORT_IF_ERROR(
      bro::AddComponent<PhysicalComponent>(
        entity_id,
        sf::Rect<float>(), 1, PhysicalGroup::DynamicObject
      )
    );
    REPORT_IF_ERROR(
      bro::AddComponent<GraphicalComponent>(
        entity_id,
        bro::GetSystem<GraphicalSystem>()->GetMiddle()
      )
    );

    bro::GetComponentUnsafe<GraphicalComponent>(entity_id)->sprite.setTexture(
      bro::GetResourceManager()->Get("img/effective-broccoli.png")
    );
    bro::GetComponentUnsafe<PhysicalComponent>(entity_id)->hitbox_ =
      bro::GetComponentUnsafe<GraphicalComponent>(entity_id)->sprite.getGlobalBounds();
  }

private:

};

#endif // EFFECTIVE_BROCCOLI_BROCCOLI_HPP
