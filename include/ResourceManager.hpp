#ifndef EFFECTIVE_BROCOLLI_RESOURCEMANAGER_HPP
#define EFFECTIVE_BROCOLLI_RESOURCEMANAGER_HPP

#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>

class ResourceManager {
private:
  ResourceManager();

  static ResourceManager* resource_manager_;
  std::unordered_map<std::string, sf::Texture> map_;
public:
  /// Load texture into the memory
  int load(const std::string& path);

  /// Get previously loaded texture
  sf::Texture& get(const std::string& path);

  ResourceManager(ResourceManager &other) = delete;
  void operator=(const ResourceManager &) = delete;

  static ResourceManager *GetInstance();
};

#endif //EFFECTIVE_BROCOLLI_RESOURCEMANAGER_HPP
