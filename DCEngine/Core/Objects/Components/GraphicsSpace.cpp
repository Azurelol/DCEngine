#include "GraphicsSpace.h"
#include "ObjectReferences.h"
#include "../../Systems/Graphics/Graphics.h" // Access to the graphics system

namespace DCEngine {

  void GraphicsSpace::Initialize() {
    // Register self to the graphics system
    Daisy->GetSystem<Systems::Graphics>(EnumeratedSystem::Graphics)->Register(*this);

  }

  std::vector<GameObject>& GraphicsSpace::getSprites() {
    return sprites_;
  }

}