#pragma once
#include "..\Component.h"

namespace DCEngine {

  class Sprite : public Component {
    Sprite(Entity& owner) : Component(EnumeratedComponent::Sprite, BitfieldComponent::Sprite, owner) {}


  };

}
