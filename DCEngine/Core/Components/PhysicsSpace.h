#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class PhysicsSpace : public Component {
  public:
    PhysicsSpace(Entity& owner);
    void Initialize();
    void OnLogicUpdate(Event* updateEvent);

  };


}