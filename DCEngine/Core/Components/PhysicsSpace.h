#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class PhysicsSpace : public Component {
  public:
    PhysicsSpace(Entity& owner) : Component(std::string("PhysicsSpace"), 
                                  EnumeratedComponent::PhysicsSpace, 
                                  BitfieldComponent::PhysicsSpace, owner) {}
    void Initialize();
    void OnLogicUpdate(Event* updateEvent);

  };


}