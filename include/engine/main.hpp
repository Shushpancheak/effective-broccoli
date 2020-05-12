/** main.hpp
 * Provides an interface to main functionality of the engine.
 */

#ifndef BROCCOLI_ENGINE_HPP
#define BROCCOLI_ENGINE_HPP

#include "core.hpp"

#include "systems/GraphicalSystem.hpp"
#include "systems/TransformSystem.hpp"
#include "systems/PhysicalSystem.hpp"
#include <systems/ViewSystem.hpp>

#include "support/random.hpp"

namespace bro {

  inline Status Init() {
    detail::window.Construct(sf::VideoMode(defaults::WINDOW_WIDTH, defaults::WINDOW_HEIGHT), defaults::WINDOW_TITLE);

    detail::resource_man.Construct();

    detail::entity_man.Construct();

    detail::component_man.Construct();
    detail::system_man.Construct();

    detail::event_man.Construct(GetSystemManager());


    GetSystemManager()->AddSystem<GraphicalSystem>().ThrowIfError();
    GetSystemManager()->AddSystem<TransformSystem>().ThrowIfError();
    GetSystemManager()->AddSystem<PhysicalSystem>() .ThrowIfError();
    GetSystemManager()->AddSystem<ViewSystem>(bro::GetWindow()).ThrowIfError();

    rnd::Init(time(nullptr)).ThrowIfError();

    return make_result::Ok();
  }

  inline Status Terminate() {
    rnd::Terminate().ExpectOk();

    detail::system_man.Destroy();

    detail::event_man.Destroy();
    detail::component_man.Destroy();

    detail::entity_man.Destroy();

    detail::resource_man.Destroy();

    detail::window.Destroy();

    return make_result::Ok();
  }

}

#endif // BROCCOLI_ENGINE_HPP
