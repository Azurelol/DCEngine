#include "SoundEmitter.h"

namespace DCEngine {
  
  void SoundEmitter::Initialize()
  {

  }

  void SoundEmitter::Serialize(Json::Value & root)
  {

  }

  void SoundEmitter::Deserialize(Json::Value & root)
  {
  }

  void SoundEmitter::PlayCue(String soundCue)
  {
    CurrentSoundCue = soundCue;

  }

  void SoundEmitter::Stop()
  {
  }

  void SoundEmitter::setVolume()
  {
  }

  void SoundEmitter::getVolume()
  {
  }


}