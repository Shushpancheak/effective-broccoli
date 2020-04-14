#include "Entity.hpp"
#include "SFML/Graphics.hpp"
#include "constants/error.hpp"
#include "managers/ComponentManager.hpp"
#include <iostream>


int main() {
  ComponentManager a;
  sf::RenderWindow window(sf::VideoMode(200, 200), "effective-brocolli");

  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);

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