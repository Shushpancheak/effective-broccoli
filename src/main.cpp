#include "SFML/Graphics.hpp"
#include <iostream>

#include "managers/ComponentManager.hpp"
#include "managers/EventManager.hpp"
#include "managers/EntityManager.hpp"
#include "managers/ResourceManager.hpp"
#include "managers/SystemManager.hpp"

int main() {
  sf::RenderWindow window(sf::VideoMode(200, 200), "effective-brocolli");

  ResourceManager resource_manager;

  EntityManager entity_manager;
  ComponentManager component_manager;

  SystemManager system_manager(window);
  EventManager event_manager(&system_manager);

  while (window.isOpen()) {
    sf::Event event{};

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear();


    window.draw(shape);


    window.display();
  }

  return 0;
}