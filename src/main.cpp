#include "SFML/Graphics.hpp"
#include <iostream>
#include <thread>

#include "engine/main.hpp"

#include "entities/Broccoli.hpp"

int main() {
  bro::Init().ThrowIfError();

  for (size_t i = 0; i < 10; ++i) {
    bro::GetEntityManager()->AddEntity<BroccoliEntity>(
      sf::Vector2f(rnd::GetFloatRand() * 500, rnd::GetFloatRand() * 500)
    ).ThrowIfError();
  }

  const Duration time_for_frame(static_cast<long long>((1e9 / static_cast<float>(defaults::FPS))));

  while (bro::GetWindow()->isOpen()) {
    sf::Event event{};

    while (bro::GetWindow()->pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        bro::GetWindow()->close();
      }
    }

    bro::GetWindow()->clear();

    bro::GetSystem<GraphicalSystem>()->UpdateBase();
    bro::GetSystem<PhysicalSystem>()->UpdateBase();

    bro::GetEventManager()->RunFor(time_for_frame / 10);

    std::this_thread::sleep_for(9 * time_for_frame / 10);

    bro::GetWindow()->display();
  }

  bro::Terminate().ThrowIfError();

  return 0;
}