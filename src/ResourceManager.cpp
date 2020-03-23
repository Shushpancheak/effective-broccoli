#include "ResourceManager.hpp"

ResourceManager* ResourceManager::resource_manager_= nullptr;

ResourceManager *ResourceManager::GetInstance() {
  if (resource_manager_ == nullptr) {
    resource_manager_ = new ResourceManager();
  }
  return resource_manager_;
}

ResourceManager::ResourceManager() {
  std::ifstream infile("resources/essentials.txt");
  std::string line;
  while (std::getline(infile, line)) {
    load(line);
  }
}

int ResourceManager::load(const std::string &path) {
  sf::Texture texture;
  if (!texture.loadFromFile(path)) {
    return 1;
  }
  map_[path] = texture;
  return 0;
}

sf::Texture &ResourceManager::get(const std::string &path) {
  return map_[path];
}
