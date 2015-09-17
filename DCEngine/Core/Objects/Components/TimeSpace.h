#pragma once
#include "..\Component.h"

namespace DCEngine {

  class TimeSpace : public Component {
  public:
    TimeSpace(Entity& owner) : Component(std::string("TimeSpace"), EnumeratedComponent::TimeSpace, BitfieldComponent::TimeSpace, owner) {}
    void Initialize();
    void OnLogicUpdate(Event* updateEvent);
    

  };

}
