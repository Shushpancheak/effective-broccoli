#ifndef EFFECTIVE_BROCOLLI_BACKGROUND_HPP
#define EFFECTIVE_BROCOLLI_BACKGROUND_HPP

#include "Entity.hpp"

#include "engine/core.hpp"
#include "components/GraphicalComponent.hpp"
#include "systems/GraphicalSystem.hpp"

class BackgroundEntity : public Entity {
public:
  static const EntityID type_id = ENTITY_BACKGROUND;

  int32_t padding;

  BackgroundEntity(const EntityID entity_id)
      : Entity(entity_id) {
    REPORT_IF_ERROR(
        bro::AddComponent<GraphicalComponent>(
            entity_id,
            bro::GetSystem<GraphicalSystem>()->GetMiddle()
        )
    );

    bro::GetComponentUnsafe<GraphicalComponent>(entity_id)->sprite.setTexture(
        bro::GetResourceManager()->Get("img/background1.jpg")
    );
  }

private:

};

#endif //EFFECTIVE_BROCOLLI_BACKGROUND_HPP
