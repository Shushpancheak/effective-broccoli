#ifndef EFFECTIVE_BROCCOLI_DEFAULTS_HPP
#define EFFECTIVE_BROCCOLI_DEFAULTS_HPP

namespace defaults {

enum {
  WINDOW_WIDTH = 1280,
  WINDOW_HEIGHT = 720,
  FPS = 60
};

const char WINDOW_TITLE[] = "effective-broccoli";

const sf::Rect<float> WORLD_BOUNDARY = {0, 0, 200, 200};

#ifdef _DEBUG

#ifdef WIN32
const std::string_view RESOURCES_FOLDER = "../../../resources/";
#else
const std::string_view RESOURCES_FOLDER = "../resources/";
#endif

#else
const std::string_view RESOURCES_FOLDER = "resources/";
#endif

}

#endif // EFFECTIVE_BROCCOLI_DEFAULTS_HPP
