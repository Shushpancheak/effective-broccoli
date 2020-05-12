#include "SFML/Graphics.hpp"
#include <iostream>
#include <thread>

#include "engine/main.hpp"

#include "entities/Broccoli.hpp"
#include "entities/Background.hpp"

#include "events/view_events.hpp"

int main() {
  bro::Init().ThrowIfError();

  bro::GetEntityManager()->AddEntity<BackgroundEntity>().ThrowIfError();

  int player_id = 0;
  for (size_t i = 0; i < 10; ++i) {
    player_id = bro::GetEntityManager()->AddEntity<BroccoliEntity>(
      sf::Vector2f(rnd::GetFloatRand() * 500, rnd::GetFloatRand() * 500)
    ).Value();
  }


  const Duration time_for_frame(static_cast<long long>((1e9 / static_cast<float>(defaults::FPS))));

  while (bro::GetWindow()->isOpen()) {
    sf::Event event{};

    while (bro::GetWindow()->pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        bro::GetWindow()->close();
      }
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
          bro::RegisterEvent<MoveEvent>(player_id, sf::Vector2f(0, -25));
          const auto reg_res = bro::RegisterEvent<MoveViewEvent>(sf::Vector2f(0, -25));
        }
        if (event.key.code == sf::Keyboard::Left) {
          bro::RegisterEvent<MoveEvent>(player_id, sf::Vector2f(-25, 0));
          const auto reg_res = bro::RegisterEvent<MoveViewEvent>(sf::Vector2f(-25, 0));
        }
        if (event.key.code == sf::Keyboard::Right) {
          bro::RegisterEvent<MoveEvent>(player_id, sf::Vector2f(25, 0));
          const auto reg_res = bro::RegisterEvent<MoveViewEvent>(sf::Vector2f(25, 0));
        }
        if (event.key.code == sf::Keyboard::Down) {
          bro::RegisterEvent<MoveEvent>(player_id, sf::Vector2f(0, 25));
          const auto reg_res = bro::RegisterEvent<MoveViewEvent>(sf::Vector2f(0, 25));
        }
        if (event.key.code == sf::Keyboard::Q) {
          const auto res_reg = bro::RegisterEvent<RotateViewEvent>(5);
        }
        if (event.key.code == sf::Keyboard::E) {
          const auto res_reg = bro::RegisterEvent<RotateViewEvent>(-5);
        }
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