#pragma once
#include "ComponentReference.h"

namespace DCEngine {
  
  class Sprite;
  class DebugFade : public Component {
  public:
    
    Sprite* SpriteComponent;
    Real FadeOutDur = 10;
    Real4 FadeColor = Real4(0, 0, 0, 0);

    /* Initialize */
    DebugFade(Entity& owner) : Component(std::string("DebugFade"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

    /* Events */
    void OnLogicUpdateEvent(Events::LogicUpdate* event);

  private:
  };
}