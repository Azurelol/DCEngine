#include "SoundCue.h"
#include "../Components/EngineReference.h"

namespace DCEngine {
  
  /**************************************************************************/
  /*!
  @brief  SoundCue constructor.
  */
  /**************************************************************************/
  SoundCue::SoundCue(std::string soundFile) : Resource("SoundCue"), SoundFileName(soundFile) {
    //Load();
  }

  /**************************************************************************/
  /*!
  @brief  Loads this Sound pointer by interfacing with the audio system.
  */
  /**************************************************************************/
  void SoundCue::Load()
  {
    Daisy->getSystem<Systems::Audio>()->CreateSound(std::string(SoundFileName), SoundPtr);
  }

}