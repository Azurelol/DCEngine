#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class SoundEmitter : public Component {
  public:

    Real Volume = 1;
    Real Pitch = 1;
    Boolean IsPlaying = false;

    void PlayCue(String soundCue);
    void Stop();
    void setVolume();
    void getVolume();

    /* Initialize */
    SoundEmitter(Entity& owner) : Component(std::string("SoundEmitter"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

  private:
    String CurrentSoundCue; //!< The 'SoundCue' that this emitter currently holds.

  };
}