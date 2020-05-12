#ifndef EFFECTIVE_BROCCOLI_TRANSFORM_SYSTEM
#define EFFECTIVE_BROCCOLI_TRANSFORM_SYSTEM

#include "System.hpp"

#include "events/transform_events.hpp"
#include "events/graphic_events.hpp"
#include "components/TransformComponent.hpp"

#include "engine/core.hpp"

class TransformSystem : public System {
public:
  const static SystemID type_id = SYSTEM_TRANSFORM;

  explicit TransformSystem();
  ~TransformSystem() {};
  virtual void Accept(EventPtr event_ptr) override;

private:
  virtual void Update(Duration delta_time) override;
};

#endif