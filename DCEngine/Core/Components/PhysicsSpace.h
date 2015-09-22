#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class PhysicsSpace : public Component {
  public:

    bool AllowSleep;
    bool Mode2D;
    bool Deterministic;
    
    PhysicsSpace(Entity& owner);
    void Initialize();
    void OnLogicUpdate(Event* updateEvent);

  };


}