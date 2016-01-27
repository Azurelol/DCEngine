/******************************************************************************/
/*!
@file   AudioFMOD_Playback.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   1/24/2016
@brief  The interface for FMOD.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "AudioFMOD.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Plays a sound through FMOD LowLevel API.
    @param  soundPtr A pointer to the Sound data.
    @param  channel  A pointer to the Channel handle.
    @param  loop Whether the sound should be played in a loop.
    */
    /**************************************************************************/
    bool AudioFMOD::PlaySound(FMOD::Sound* soundPtr, FMOD::Channel** channel, bool loop) {
      DCTrace << "AudioFMOD::PlaySound \n";
      if (loop) {
        soundPtr->setMode(FMOD_LOOP_NORMAL);
        soundPtr->setLoopCount(-1);
      }
      else {
        soundPtr->setMode(FMOD_LOOP_OFF);
      }

      //FMOD::Channel::
      return ErrorCheck(System.LowLevel->playSound(soundPtr, NULL, 0, channel));
    }

    /**************************************************************************/
    /*!
    @brief  Plays a sound through FMOD Studio API.
    @param  eventDescription The event which to play.
    */
    /**************************************************************************/
    bool AudioFMOD::PlaySound(std::string & eventName)
    {
      // If the event has already been instantiated, use it.
      if (AvailableEvents.count(eventName)) {
      }
      // Otherwise, create an instance of it.
      else {

      }
      //ErrorCheck(System->getEvent(eventDescription.c_str()));

      return true;
    }

    /**************************************************************************/
    /*!
    \brief  Resumes the playing of a sound through FMOD.
    */
    /**************************************************************************/
    void AudioFMOD::ResumeSound(FMOD::Channel * channel)
    {
      ErrorCheck(channel->setPaused(false));
    }

    /**************************************************************************/
    /*!
    \brief  Stops a sound from playing through FMOD.
    */
    /**************************************************************************/
    void AudioFMOD::PauseSound(FMOD::Channel* channel)
    {
      ErrorCheck(channel->setPaused(true));
    }

    /**************************************************************************/
    /*!
    \brief  Stops a sound from playing through FMOD.
    */
    /**************************************************************************/
    void AudioFMOD::StopSound(FMOD::Channel* channel)
    {
      ErrorCheck(channel->stop());
    }
    /**************************************************************************/
    /*!
    \brief  Releases a sound from FMOD.
    */
    /**************************************************************************/
    void AudioFMOD::ReleaseSound(FMOD::Sound* soundPtr) {
      ErrorCheck(soundPtr->release());
    }

  }
}
