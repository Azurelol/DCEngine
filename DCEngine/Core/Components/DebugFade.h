#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class DebugFade : public Component {
  public:
    
    /* Initialize */
    DebugFade(Entity& owner) : Component(std::string("DebugFade"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

    /* Events */
    void OnKeyDownEvent(Events::KeyDown* event);
    void OnKeyUpEvent(Events::KeyUp* event);
    void ChangeTrack(std::string& track);

  private:
  };
}