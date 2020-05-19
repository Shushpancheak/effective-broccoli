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

int main() {
  bro::Init().ThrowIfError();

  MapLoader(std::string(defaults::CONFIG_PATH));
  int player_id = bro::GetEntityManager()->AddEntity<BroccoliEntity>(
    sf::Vector2f(rnd::GetFloatRand() * 500, rnd::GetFloatRand() * 500)
  ).Value();
  int mob_id = bro::GetEntityManager()->AddEntity<GarnetEntity>(
      sf::Vector2f(rnd::GetFloatRand() * 500, rnd::GetFloatRand() * 500)
  ).Value();


  const Duration time_for_frame(static_cast<long long>((1e9 / static_cast<float>(defaults::FPS))));

  while (bro::GetWindow()->isOpen()) {
    sf::Event event{};
    auto player_hitbox = bro::GetComponentManager()->GetComponent<PhysicalComponent>(player_id).Value()->hitbox_;
    auto mob_hitbox = bro::GetComponentManager()->GetComponent<PhysicalComponent>(mob_id).Value()->hitbox_;
    auto mob_vel = bro::GetComponentManager()->GetComponent<PhysicalComponent>(mob_id).Value()->velocity_;

    double len = std::sqrt(mob_vel.x * mob_vel.x + mob_vel.y * mob_vel.y);
    std::cout << player_hitbox.top - mob_hitbox.top << ' ' << player_hitbox.left - mob_hitbox.left << std::endl;
    double k = (player_hitbox.top - mob_hitbox.top) / (player_hitbox.left - mob_hitbox.left);
    std::cout << "PLAYER: " << player_hitbox.top << ' ' << player_hitbox.left << std::endl;
    std::cout << "MOB: " << mob_hitbox.top << ' ' << mob_hitbox.left << std::endl;
    //std::cout << "LEN: " << len << " K: " << k << std::endl;
    double new_vel_x = std::sqrt((len * len) / (k * k + 1));
    double new_vel_y = new_vel_x * k;
    int x_sign = player_hitbox.left - mob_hitbox.left > 0? 1 : -1;
    int y_sign = player_hitbox.top - mob_hitbox.top > 0? -1 : 1;
    new_vel_y *= y_sign;
    if (std::abs(player_hitbox.left - mob_hitbox.left) < 1e-7) {
      new_vel_y = len * y_sign;
      new_vel_x = 0;
    }
    new_vel_x *= x_sign;
//    std::cout << "SPEED: " << new_vel_x << ' ' << new_vel_y << std::endl;
    bro::GetComponentManager()->GetComponent<PhysicalComponent>(mob_id).Value()->velocity_ = {static_cast<float>(new_vel_x), static_cast<float>(new_vel_y)};
    while (bro::GetWindow()->pollEvent(event)) {
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