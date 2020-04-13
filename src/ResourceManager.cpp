#include "ResourceManager.hpp"

ResourceManager* ResourceManager::resource_manager_= nullptr;

ResourceManager* ResourceManager::GetInstance() {
  if (resource_manager_ == nullptr) {
    resource_manager_ = new ResourceManager();
  }
  return resource_manager_;
}

ResourceManager::ResourceManager() {
  std::ifstream infile("resources/essentials.txt");
  std::string line;
  while (std::getline(infile, line)) {
    (void)Load(line);
  }
}

Status ResourceManager::Load(const std::string& path) {
  sf::Texture texture;
  if (!texture.loadFromFile(path)) {
    return make_result::Fail(LOAD_ERROR);
  }
  map_[path] = texture;
  return make_result::Ok();
}

sf::Texture& ResourceManager::Get(const std::string& path) {
  return map_[path];
}
