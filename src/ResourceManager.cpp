#include "managers/ResourceManager.hpp"

ResourceManager::ResourceManager() {
  std::string path = std::string(defaults::RESOURCES_FOLDER) + std::string("essentials.txt");

  std::system("echo %cd%");

  std::ifstream infile(path);
  std::string line;
  while (std::getline(infile, line)) {
    Load(line).ThrowIfError();
  }
}

Status ResourceManager::Load(const std::string& path) {
  sf::Texture texture;
  // TODO make it CHARRRRRRRRRRRRRRRRRRRRRRR[]
  if (!texture.loadFromFile(std::string(defaults::RESOURCES_FOLDER) + path)) {
    return make_result::Fail(LOAD_ERROR);
  }
  map_[path] = texture;
  return make_result::Ok();
}

sf::Texture& ResourceManager::Get(const std::string& path) {
  return map_[path];
}
