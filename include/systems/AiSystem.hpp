#ifndef EFFECTIVE_BROCCOLI_AI_SYSTEM
#define EFFECTIVE_BROCCOLI_AI_SYSTEM

#include "System.hpp"

#include "engine/core.hpp"
#include "events/physical_events.hpp"
#include "components/AiComponent.hpp"
#include "components/GraphicalComponent.hpp"

class AiSystem : public System {
public:
  const static SystemID type_id = SYSTEM_AI;

  explicit AiSystem();
  ~AiSystem() = default;;
  virtual void Accept(EventPtr event_ptr) override;

private:
  virtual void Update(Duration delta_time) override;
};

#endif