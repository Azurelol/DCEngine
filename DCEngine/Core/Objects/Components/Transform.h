#pragma once
#include "..\Component.h"

namespace DCEngine {
  
  class Transform : public Component {
  public:
    Transform() : Component(EnumeratedComponent::Transform, BitfieldComponent::Transform) {

    }



  };

}