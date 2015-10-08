#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class DebugAudio : public Component {
  public:
    
    String SoundCue1 = "spacejam";
    String SoundCue2 = "spacejam1";

    /* Initialize */
    DebugAudio(Entity& owner) : Component(std::string("DebugAudio"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);
    
    /* Events */
    void OnKeyDownEvent(Events::KeyDown* event);
    void OnKeyUpEvent(Events::KeyUp* event);

  private:
  };
}