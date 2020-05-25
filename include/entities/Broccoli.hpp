#ifndef EFFECTIVE_BROCCOLI_BROCCOLI_HPP
#define EFFECTIVE_BROCCOLI_BROCCOLI_HPP

#include "Entity.hpp"

#include "engine/core.hpp"
#include "components/TransformComponent.hpp"
#include "components/PhysicalComponent.hpp"
#include "support/random.hpp"

class BroccoliEntity : public Entity {
public:
  static const EntityID type_id = ENTITY_BROCCOLI;

  int32_t padding;

  BroccoliEntity(const EntityID entity_id, const sf::Vector2f initial_pos)
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

    //REPORT_IF_ERROR(
    //    bro::RegisterEvent<SetTransformEvent>(
    //        entity_id,
    //        sf::Transform().translate(initial_pos)
    //    )
    //);

    bro::GetComponentUnsafe<GraphicalComponent>(entity_id)->sprite.setPosition(initial_pos);

    bro::GetComponentUnsafe<GraphicalComponent>(entity_id)->sprite.setTexture(
      bro::GetResourceManager()->Get("img/effective-broccoli.png")
    );
//    bro::GetComponentUnsafe<GraphicalComponent>(entity_id)->sprite.setScale(
//      rnd::GetVector2fRand() * 0.5f
//    );


    bro::GetComponentUnsafe<PhysicalComponent>(entity_id)->force_ = {0, 0};
    bro::GetComponentUnsafe<PhysicalComponent>(entity_id)->velocity_ = {100, -100};

    auto bounds = bro::GetComponentUnsafe<GraphicalComponent>(entity_id)->sprite.getGlobalBounds();
    bro::GetComponentUnsafe<PhysicalComponent>(entity_id)->hitbox_ =
      bounds;
  }

private:

};

#endif // EFFECTIVE_BROCCOLI_BROCCOLI_HPP
