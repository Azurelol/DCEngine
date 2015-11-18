#include "SoundEmitter.h"
#include "EngineReference.h"

namespace DCEngine {
    void SoundEmitter::Initialize()
    {

    }

    /**************************************************************************/
    /*!
    @brief  Plays a 'SoundCue' by sending a request to the SoundSpace.
    @param  soundCue The name of the 'SoundCue' to play.
    */
    /**************************************************************************/
    void SoundEmitter::PlayCue(String soundCue)
    {
      CurrentSoundCue = soundCue;
      SpaceRef->getComponent<SoundSpace>()->PlayCue(soundCue);
    }

    /**************************************************************************/
    /*!
    @brief  Stops the currently playing 'Soundue'
    */
    /**************************************************************************/
    void SoundEmitter::Stop()
    {
      SpaceRef->getComponent<SoundSpace>()->StopCue(CurrentSoundCue);
    }

    void SoundEmitter::setVolume()
    {
    }

    void SoundEmitter::getVolume()
    {
    }

  
}