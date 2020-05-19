#ifndef EFFECTIVE_BROCOLLI_VIEWSYSTEM_HPP
#define EFFECTIVE_BROCOLLI_VIEWSYSTEM_HPP

#include "SFML/Graphics/View.hpp"

#include "System.hpp"

//#include "events/graphic_events.hpp"

#include "engine/core.hpp"

class ViewSystem : public System {
public:
  const static SystemID type_id = SYSTEM_VIEW;

  explicit ViewSystem(sf::RenderWindow*);

  virtual void Accept(EventPtr event_ptr) override;

private:
  virtual void Update(Duration delta_time) override;

  sf::View view;
};

#endif //EFFECTIVE_BROCOLLI_VIEWSYSTEM_HPP
