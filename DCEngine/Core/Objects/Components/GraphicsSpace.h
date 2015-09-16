#pragma once
#include "..\Component.h"

namespace DCEngine {

  class GraphicsSpace : public Component {
  public:
    GraphicsSpace(Entity& owner) : Component(std::string("GraphicsSpace"),
                                   EnumeratedComponent::GraphicsSpace,
                                   BitfieldComponent::GraphicsSpace, owner) {}
    void Initialize();
    void OnLogicUpdate(Event* updateEvent);

  };

}