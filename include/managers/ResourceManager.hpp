#ifndef EFFECTIVE_BROCOLLI_RESOURCE_MANAGER_HPP
#define EFFECTIVE_BROCOLLI_RESOURCE_MANAGER_HPP

#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>

#include "constants/error.hpp"
#include "support/result.hpp"

class ResourceManager {
private:
  ResourceManager();
  ~ResourceManager() = default;

public:
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

  static ResourceManager* GetInstance();

private:
  static ResourceManager* resource_manager_;
  std::unordered_map<std::string, sf::Texture> map_;
};

#endif //EFFECTIVE_BROCOLLI_RESOURCE_MANAGER_HPP
