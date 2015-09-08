#pragma once
#include "..\Component.h"

namespace DCEngine {
  
  class Transform : public Component {
  public:
    Transform(Entity& owner) : Component(EnumeratedComponent::Transform, BitfieldComponent::Transform, owner) {}



  };

}