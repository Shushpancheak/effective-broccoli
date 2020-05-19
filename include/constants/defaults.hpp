#ifndef EFFECTIVE_BROCCOLI_DEFAULTS_HPP
#define EFFECTIVE_BROCCOLI_DEFAULTS_HPP

#include "SFML/Graphics/Rect.hpp"

namespace defaults {

enum {
  WINDOW_WIDTH = 1280,
  WINDOW_HEIGHT = 720,
  FPS = 60
};

const char WINDOW_TITLE[] = "effective-broccoli";

const sf::Rect<float> WORLD_BOUNDARY = {0, 0, 200, 200};

#ifndef NDEBUG

#ifdef WIN32
const std::string_view RESOURCES_FOLDER = "../../../resources/";
const std::string_view CONFIG_PATH = "../../../resources/config.yaml";
#else
const std::string_view RESOURCES_FOLDER = "../resources/";
const std::string_view CONFIG_PATH = "../resources/config.yaml";
#endif

#else
const std::string_view RESOURCES_FOLDER = "resources/";
const std::string_view CONFIG_PATH = "resources/config.yaml";
#endif

}

#endif // EFFECTIVE_BROCCOLI_DEFAULTS_HPP
