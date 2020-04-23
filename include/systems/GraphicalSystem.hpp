#ifndef EFFECTIVE_BROCCOLI_GRAPHICAL_SYSTEM
#define EFFECTIVE_BROCCOLI_GRAPHICAL_SYSTEM

#include "SFML/Graphics/RenderWindow.hpp"
#include "System.hpp"

#include "components/GraphicalComponent.hpp"

class GraphicalSystem : public System {
public:
  const static SystemID type_id = SYSTEM_GRAPHICAL;

  explicit GraphicalSystem(sf::RenderWindow& window);
  
  virtual void Accept(EventPtr event_ptr) override;
  virtual void Update(float delta_time) override;

private:
  sf::RenderWindow& window_;

  GraphicalComponent root_;

  GraphicalComponent background_;
  GraphicalComponent middle_;
  GraphicalComponent foreground_;
};

#endif