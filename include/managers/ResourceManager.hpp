#ifndef EFFECTIVE_BROCOLLI_RESOURCE_MANAGER_HPP
#define EFFECTIVE_BROCOLLI_RESOURCE_MANAGER_HPP

#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>

#include "constants/error.hpp"
#include "support/result.hpp"
#include "constants/defaults.hpp"

//TODO make abstract class Resource.
class ResourceManager {
public:
  ResourceManager();
  ~ResourceManager() = default;

  ResourceManager(const ResourceManager& other) = delete;
  ResourceManager(ResourceManager&& other) = delete;
  ResourceManager& operator=(const ResourceManager&) = delete;
  ResourceManager& operator=(ResourceManager&&) = delete;

  /**
   * Load texture into the memory
   */
  Status Load(const std::string& path);

  /**
   * Get previously loaded texture
   */
  sf::Texture& Get(const std::string& path);

private:
  std::unordered_map<std::string, sf::Texture> map_;
};

#endif //EFFECTIVE_BROCOLLI_RESOURCE_MANAGER_HPP
