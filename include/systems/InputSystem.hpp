//
// Created by dm on 4/28/20.
//

#ifndef EFFECTIVE_BROCOLLI_INCLUDE_SYSTEMS_INPUTSYSTEM_HPP_
#define EFFECTIVE_BROCOLLI_INCLUDE_SYSTEMS_INPUTSYSTEM_HPP_
#include <SFML/Graphics/RenderWindow.hpp>
#include "SFML/Graphics.hpp"

#include <vector>

class InputSystem {
 public:

  /**
   * Process all input events and add it to event queue.
   */
  void readAllInput();

 private:
  sf::RenderWindow& window_;

};

#endif //EFFECTIVE_BROCOLLI_INCLUDE_SYSTEMS_INPUTSYSTEM_HPP_
