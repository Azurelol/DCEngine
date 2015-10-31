#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class DebugAudio : public Component {
  public:
    
    String Track1;
    String Track2;
    String Track3;
    String CurrentSoundCue;

    /* Initialize */
    DebugAudio(Entity& owner) : Component(std::string("DebugAudio"), owner) {}
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