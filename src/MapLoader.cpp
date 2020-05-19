#include "support/MapLoader.hpp"
#include "components/GraphicalComponent.hpp"
#include "components/TransformComponent.hpp"
#include "systems/GraphicalSystem.hpp"
#include "events/transform_events.hpp"
#include "yaml-cpp/node/parse.h"

MapLoader::MapLoader(const std::string &filename) : INITIAL_X(-1000), INITIAL_Y(-1000) {
  float SCALE_FACTOR = 8;
  sf::Vector2f current_pos = {INITIAL_X, INITIAL_Y};
  blocks_ = YAML::LoadAllFromFile(filename);
  for (auto& block : blocks_) {
    if (block["graph_comp"]) {
      const auto& sprite_pos = block["graph_comp"]["sprite_pos"];

      types_.push_back({
        block["graph_comp"]["sprite_path"].as<std::string>(),
        {
          sprite_pos["left"].as<int>(),
          sprite_pos["top"].as<int>(),
          sprite_pos["width"].as<int>(),
          sprite_pos["height"].as<int>()
        }
      });
    }
  }
  for (const auto& row : blocks_.back()["Map"]["tiles"]) {
    for (auto map_entity_id : row.as<std::vector<int>>()) {
      int current_id = bro::GetEntityManager()->AddEntity<Entity>();
      bro::AddComponent<GraphicalComponent>(
        current_id,
        bro::GetSystem<GraphicalSystem>()->GetMiddle()
      ).ThrowIfError();
      bro::AddComponent<TransformComponent>(
        current_id,
        nullptr
      ).ThrowIfError();
      bro::GetComponentUnsafe<GraphicalComponent>(
        current_id
        )->sprite.setTexture(
          bro::GetResourceManager()->Get(types_[map_entity_id].sprite_path)
        );
      bro::GetComponentUnsafe<GraphicalComponent>(current_id)->sprite.setScale(
          {SCALE_FACTOR, SCALE_FACTOR}
      );
      bro::GetComponentUnsafe<GraphicalComponent>(current_id)->sprite.setTextureRect(
          types_[map_entity_id].sprite_rect
      );
      bro::RegisterEvent<SetTransformEvent>(
        current_id,
        sf::Transform().translate(current_pos)
      ).ThrowIfError();

      current_pos.x += types_[map_entity_id].sprite_rect.width * SCALE_FACTOR;
    }
    current_pos.y += 32 * SCALE_FACTOR;
    current_pos.x = INITIAL_X;
    std::cout << std::endl;
  }
}
