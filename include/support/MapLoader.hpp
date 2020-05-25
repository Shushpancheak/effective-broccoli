#ifndef EFFECTIVE_BROCOLLI_MAPLOADER_HPP
#define EFFECTIVE_BROCOLLI_MAPLOADER_HPP

#include <string>
#include <unordered_map>
#include <functional>

#include "yaml-cpp/yaml.h"
#include "engine/core.hpp"
#include "yaml-cpp/node/node.h"

struct CommonEntityData {
  std::string sprite_path;
  sf::Rect<int> sprite_rect;
  bool is_static;
};

class MapLoader {
public:
  explicit MapLoader(const std::string& filename);

private:
  std::vector<YAML::Node> blocks_;
  std::vector<CommonEntityData> types_;
  float INITIAL_X;
  float INITIAL_Y;
};


#endif //EFFECTIVE_BROCOLLI_MAPLOADER_HPP
