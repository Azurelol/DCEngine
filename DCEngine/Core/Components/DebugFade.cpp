#include "DebugFade.h"
#include "EngineReference.h"
#include "Sprite.h"

namespace DCEngine {

  void DebugFade::Initialize()
  {
    SpriteComponent = Owner()->getComponent<Sprite>();
    Connect(SpaceRef, Events::LogicUpdate, DebugFade::OnLogicUpdateEvent);
  }

  void DebugFade::Serialize(Json::Value & root)
  {
  }

  void DebugFade::Deserialize(Json::Value & root)
  {
  }

  void DebugFade::OnLogicUpdateEvent(Events::LogicUpdate * event)
  {
    // Decrement the alpha every update
    auto decrement = 0.001; // lol
    auto fadedColor = SpriteComponent->Color.a - decrement;
    // lol 2
    if (fadedColor < 0)
      return;

    SpriteComponent->Color = Real4(SpriteComponent->Color.x,
                                   SpriteComponent->Color.y,
                                   SpriteComponent->Color.z,
                                   fadedColor);
  }


}