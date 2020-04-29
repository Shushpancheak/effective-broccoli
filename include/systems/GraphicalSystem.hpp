#ifndef EFFECTIVE_BROCCOLI_GRAPHICAL_SYSTEM
#define EFFECTIVE_BROCCOLI_GRAPHICAL_SYSTEM

#include "SFML/Graphics/RenderWindow.hpp"

#include "System.hpp"

#include "components/GraphicalComponent.hpp"
#include "events/graphic_events.hpp"
#include "entities/GraphicalPlaceholderEntity.hpp"

#include "engine/core.hpp"

class GraphicalSystem : public System {
public:
  const static SystemID type_id = SYSTEM_GRAPHICAL;

  GraphicalComponent* GetBackground() { return &background_; }
  GraphicalComponent* GetMiddle    () { return &middle_    ; }
  GraphicalComponent* GetForeground() { return &foreground_; }

  explicit GraphicalSystem();
  
  virtual void Accept(EventPtr event_ptr) override;

private:
  virtual void Update(Duration delta_time) override;

  static void UpdateRecursive(GraphicalComponent* node, GraphicalComponent* prev);

  GraphicalComponent root_;

  GraphicalComponent background_;
  GraphicalComponent middle_;
  GraphicalComponent foreground_;
};

#endif