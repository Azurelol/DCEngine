#include "GraphicsSpace.h"
#include "EngineReference.h"
#include "../Systems/Graphics/Graphics.h" // Access to the graphics system

namespace DCEngine {



  void GraphicsSpace::Initialize() {
    // Register self to the graphics system
    Daisy->getSystem<Systems::Graphics>(EnumeratedSystem::Graphics)->Register(*this);   
    // Connect to sprite registration events
    Connect(space_, Events::SpriteRegistration, GraphicsSpace::OnSpriteRegistrationEvent);

  }

  void GraphicsSpace::Serialize(Json::Value & root) {
  }

  void GraphicsSpace::Deserialize(Json::Value & root) {
  }

  void GraphicsSpace::OnSpriteRegistrationEvent(Events::SpriteRegistration* eventObj) {
    sprites_.push_back(eventObj->SpriteObj);
    trace << "GraphicsSpace::OnSpriteRegistrationEvent - " 
          << eventObj->SpriteObj->Name() << "\n";
  }

  void GraphicsSpace::DrawCircle(Real3 pos, Real radius, Real4 color)
  {
    Daisy->getSystem<Systems::Graphics>(EnumeratedSystem::Graphics)->DrawCircle(pos, radius, color);
  }

  void GraphicsSpace::DrawRectangle(Real3 pos, Real width, Real height, Real4 color)
  {
    Daisy->getSystem<Systems::Graphics>(EnumeratedSystem::Graphics)->DrawRectangle(pos, width, height, color);
  }

  void GraphicsSpace::DrawLineSegment(Real3 startPos, Real3 endPos, Real length, Real4 color)
  {
    Daisy->getSystem<Systems::Graphics>(EnumeratedSystem::Graphics)->DrawLineSegment(startPos, endPos, length, color);
  }


  std::vector<GameObject*> GraphicsSpace::getSprites() {
    return sprites_;
  }

}