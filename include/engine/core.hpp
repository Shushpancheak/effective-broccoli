/** core.hpp
 * Provides an interface for managers.
 */

#ifndef BROCCOLI_ENGINE_CORE_HPP
#define BROCCOLI_ENGINE_CORE_HPP

#include "support/result.hpp"

#include "constants/defaults.hpp"

#include "managers/ComponentManager.hpp"
#include "managers/EventManager.hpp"
#include "managers/EntityManager.hpp"
#include "managers/ResourceManager.hpp"
#include "managers/SystemManager.hpp"

namespace bro {

  namespace detail {

    inline ValueStorage<sf::RenderWindow> window;

    inline ValueStorage<ResourceManager> resource_man;

    inline ValueStorage<EntityManager> entity_man;

    inline ValueStorage<ComponentManager> component_man;
    inline ValueStorage<EventManager> event_man;
    inline ValueStorage<SystemManager> system_man;

  }

  inline sf::RenderWindow* GetWindow() {
    return detail::window.PtrUnsafe();
  }

  inline ResourceManager* GetResourceManager() {
    return detail::resource_man.PtrUnsafe();
  }

  inline EntityManager* GetEntityManager() {
    return detail::entity_man.PtrUnsafe();
  }

  inline ComponentManager* GetComponentManager() {
    return detail::component_man.PtrUnsafe();
  }

  inline EventManager* GetEventManager() {
    return detail::event_man.PtrUnsafe();
  }

  inline SystemManager* GetSystemManager() {
    return detail::system_man.PtrUnsafe();
  }

  template<typename T>
  inline T* GetSystem() {
    return detail::system_man.PtrUnsafe()->GetSystem<T>();
  }

  inline System* GetSystem(const SystemID sys_id) {
    return detail::system_man.PtrUnsafe()->GetSystem(sys_id);
  }

  template<typename T>
  inline T* GetComponentUnsafe(const EntityID entity_id) {
    return detail::component_man.PtrUnsafe()->GetComponentUnsafe<T>(entity_id);
  }

  template<typename T>
  inline Result<T*> GetComponent(const EntityID entity_id) {
    return detail::component_man.PtrUnsafe()->GetComponent<T>(entity_id);
  }

  template<typename T, typename ...Args>
  inline Status AddComponent(const EntityID entity_id, Args&&... args) {
    return detail::component_man.PtrUnsafe()->
             AddComponent<T>(entity_id, std::forward<Args>(args)...);
  }

  template<typename T, typename... Args>
  inline Status RegisterEvent(Args&&... args) {
    return detail::event_man.PtrUnsafe()->RegisterEvent<T>(std::forward<Args>(args)...);
  }
}

#endif