#include "SFML/Graphics.hpp"
#include <cmath>
#include <iostream>
#include <thread>

#include "engine/main.hpp"
#include "support/MapLoader.hpp"

#include "entities/Broccoli.hpp"
#include "entities/Garnet.hpp"
#include "entities/Background.hpp"
#include "constants/defaults.hpp"
#include "events/view_events.hpp"

void HandleKeyboard(const sf::Event& event, int player_id);

int main() {
  bro::Init().ThrowIfError();

  MapLoader(std::string(defaults::CONFIG_PATH));
  int player_id = bro::GetEntityManager()->AddEntity<BroccoliEntity>(
    sf::Vector2f(0, 0)
  ).Value();
  int mob_id = bro::GetEntityManager()->AddEntity<GarnetEntity>(
      player_id,
      sf::Vector2f(1000, 1000)
  ).Value();


  const Duration time_for_frame(static_cast<long long>((1e9 / static_cast<float>(defaults::FPS))));

  while (bro::GetWindow()->isOpen()) {

    bro::GetSystem<AiSystem>()->UpdateBase();

    sf::Event event{};
    while (bro::GetWindow()->pollEvent(event)) {
      HandleKeyboard(event, player_id);
    }

    bro::GetWindow()->clear();

    bro::GetSystem<GraphicalSystem>()->UpdateBase();
    bro::GetSystem<PhysicalSystem>()->UpdateBase();

    bro::GetEventManager()->RunFor(time_for_frame / 10);

    //sf::RectangleShape shape_player({player_hitbox.width, player_hitbox.height});
    //sf::RectangleShape shape_mob   ({mob_hitbox   .width, mob_hitbox   .height});
    //shape_player.move              ({player_hitbox.left, player_hitbox.top});
    //shape_mob   .move              ({mob_hitbox   .left, mob_hitbox   .top});
    //
    //bro::GetWindow()->draw(shape_player);
    //bro::GetWindow()->draw(shape_mob   );

    std::this_thread::sleep_for(9 * time_for_frame / 10);

    bro::GetWindow()->display();
  }

  bro::Terminate().ThrowIfError();

  return 0;
}

void HandleKeyboard(const sf::Event& event, int player_id) {
  if (event.type == sf::Event::Closed) {
    bro::GetWindow()->close();
  }
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Up) {
      const auto reg_res = bro::RegisterEvent<MoveViewEvent>(sf::Vector2f(0, -25));
      bro::GetComponentManager()->GetComponent<PhysicalComponent>(player_id).Value()->velocity_.y = -150;
    }
    if (event.key.code == sf::Keyboard::Left) {
      const auto reg_res = bro::RegisterEvent<MoveViewEvent>(sf::Vector2f(-25, 0));
      bro::GetComponentManager()->GetComponent<PhysicalComponent>(player_id).Value()->velocity_.x = -150;
    }
    if (event.key.code == sf::Keyboard::Right) {
      const auto reg_res = bro::RegisterEvent<MoveViewEvent>(sf::Vector2f(25, 0));
      bro::GetComponentManager()->GetComponent<PhysicalComponent>(player_id).Value()->velocity_.x = 150;
    }
    if (event.key.code == sf::Keyboard::Down) {
      const auto reg_res = bro::RegisterEvent<MoveViewEvent>(sf::Vector2f(0, 25));
      bro::GetComponentManager()->GetComponent<PhysicalComponent>(player_id).Value()->velocity_.y = 150;
    }
    if (event.key.code == sf::Keyboard::S) {
      bro::GetComponentManager()->GetComponent<PhysicalComponent>(player_id).Value()->velocity_ = {0, 0};
    }
    if (event.key.code == sf::Keyboard::Q) {
      const auto res_reg = bro::RegisterEvent<RotateViewEvent>(5);
    }
    if (event.key.code == sf::Keyboard::E) {
      const auto res_reg = bro::RegisterEvent<RotateViewEvent>(-5);
    }
  }
}