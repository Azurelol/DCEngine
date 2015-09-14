#pragma once
#include "..\Component.h"

namespace DCEngine {

  class TimeSpace : public Component {
    TimeSpace(Entity& owner) : Component(EnumeratedComponent::TimeSpace, BitfieldComponent::TimeSpace, owner) {}
    void Initialize();
    void OnLogicUpdate(Event* updateEvent);

  };

}
