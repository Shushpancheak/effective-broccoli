//
// Created by alex on 5/19/20.
//

#ifndef EFFECTIVE_BROCOLLI_GARNET_HPP
#define EFFECTIVE_BROCOLLI_GARNET_HPP

#include "Entity.hpp"

#include "engine/core.hpp"
#include "constants/error.hpp"
#include "components/TransformComponent.hpp"
#include "components/PhysicalComponent.hpp"
#include "components/GraphicalComponent.hpp"
#include "systems/GraphicalSystem.hpp"
#include "events/transform_events.hpp"
#include "support/random.hpp"

class GarnetEntity : public Entity {
public:
  static const EntityID type_id = ENTITY_BROCCOLI;

  int32_t padding;

  GarnetEntity(const EntityID entity_id, const sf::Vector2f initial_pos)
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

    REPORT_IF_ERROR(
        bro::RegisterEvent<SetTransformEvent>(
            entity_id,
            sf::Transform().translate(initial_pos)
        )
    );

    bro::GetComponentUnsafe<GraphicalComponent>(entity_id)->sprite.setTexture(
        bro::GetResourceManager()->Get("img/angry-boi.png")
    );
    bro::GetComponentUnsafe<PhysicalComponent>(entity_id)->velocity_ = {100, 100};

    bro::GetComponentUnsafe<PhysicalComponent>(entity_id)->hitbox_ =
        bro::GetComponentUnsafe<GraphicalComponent>(entity_id)->sprite.getGlobalBounds();
  }

private:

};

#endif //EFFECTIVE_BROCOLLI_GARNET_HPP
