#ifndef EFFECTIVE_BROCOLLI_MAPLOADER_HPP
#define EFFECTIVE_BROCOLLI_MAPLOADER_HPP

#include <string>
#include <unordered_map>
#include <functional>

#include "yaml-cpp/yaml.h"
#include "engine/core.hpp"
#include "yaml-cpp/node/node.h"

struct CommonGraphicalData {
  std::string sprite_path;
  sf::Vector2f size;
};

class MapLoader {
public:
  explicit MapLoader(const std::string& filename);

private:
  std::vector<YAML::Node> blocks_;
  std::vector<CommonGraphicalData> types_;
  float INITIAL_X;
  float INITIAL_Y;
};


#endif //EFFECTIVE_BROCOLLI_MAPLOADER_HPP
