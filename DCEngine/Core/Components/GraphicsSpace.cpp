#include "GraphicsSpace.h"
#include "EngineReference.h"
#include "../Systems/Graphics/Graphics.h" // Access to the graphics system

namespace DCEngine {



  void GraphicsSpace::Initialize() {
    // Register self to the graphics system
    Daisy->GetSystem<Systems::Graphics>(EnumeratedSystem::Graphics)->Register(*this);   
    // Connect to sprite registration events
    Connect(space_, Events::SpriteRegistration, GraphicsSpace::OnSpriteRegistrationEvent);

  }

  void GraphicsSpace::OnSpriteRegistrationEvent(Events::SpriteRegistration* eventObj) {
    sprites_.push_back(eventObj->SpriteObj);
    trace << "GraphicsSpace::OnSpriteRegistrationEvent - " 
          << eventObj->SpriteObj->Name() << "\n";
  }

  std::vector<GameObject*> GraphicsSpace::getSprites() {
    return sprites_;
  }

}