#include "SoundCue.h"
#include "../Components/EngineReference.h"

namespace DCEngine {
  
  /**************************************************************************/
  /*!
  @brief  SoundCue constructor.
  */
  /**************************************************************************/
  SoundCue::SoundCue(std::string soundFile) : Resource(FileSystem::FileNoExtension(soundFile)), 
                                              SoundFileName(soundFile) {
    //Load();
  }

  /**************************************************************************/
  /*!
  @brief  Loads this Sound pointer by interfacing with the audio system.
  */
  /**************************************************************************/
  void SoundCue::Load()
  {
    Daisy->getSystem<Systems::Audio>()->CreateSound(std::string(SoundFileName), Data);
    DCTrace << "SoundCue::Load - Finished loading " << SoundFileName << "!\n";
  }

}