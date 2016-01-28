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
    bool AudioFMOD::PlaySound(FMOD::Sound* handle, FMOD::Channel** channel, PlaybackSettings& settings) {
      DCTrace << "AudioFMOD::PlaySound \n";
      if (settings.Loop) {
        handle->setMode(FMOD_LOOP_NORMAL);
        handle->setLoopCount(-1);
      }
      else {
        handle->setMode(FMOD_LOOP_OFF);
      }
      // Configure it
      auto check = ErrorCheck(System.LowLevel->playSound(handle, NULL, 0, channel));
      (*channel)->setVolume(settings.Volume);
      (*channel)->setPitch(settings.Pitch);
      return check;
    }

    /**************************************************************************/
    /*!
    @brief  Plays a sound through FMOD Studio API.
    @param  eventDescription The event which to play.
    */
    /**************************************************************************/
    bool AudioFMOD::PlaySound(EventDescriptionHandle & eventName, PlaybackSettings& settings)
    {
      // If the event has already been instantiated, use it.
      if (InstantiatedEvents.count(eventName)) {
        InstantiatedEvents.at(eventName)->start();        
        // Maybe release...
        return true;
      }

      // Otherwise, create an instance of it.
      else {
        if (AvailableEvents.count(eventName)) {
          auto eventInstance = AddEventInstance(AvailableEvents.at(eventName));
          // Configure it
          eventInstance->setVolume(settings.Volume);
          eventInstance->setPitch(settings.Pitch);          
          // One-shot sound
          eventInstance->start();
          // Release will clean up the instance when it completes
          eventInstance->release();
          return true;
        }
      }

      // Event could not be found
      DCTrace << "AudioFMOD::PlaySound: '" << eventName << "' could not be found!\n";
      return false;
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
    \brief  Stops a sound from playing through FMOD Low Level.
    */
    /**************************************************************************/
    void AudioFMOD::StopSound(FMOD::Channel* channel)
    {
      ErrorCheck(channel->stop());
    }

    /**************************************************************************/
    /*!
    \brief  Stops a sound from playing through FMOD Studio.
    */
    /**************************************************************************/
    void AudioFMOD::StopSound(EventDescriptionHandle & eventName)
    {
      DCTrace << "AudioFMOD::StopSound (Event) \n";

      // If the event has already been instantiated, use it.
      if (InstantiatedEvents.count(eventName)) {
        // Get an iterator to the event in the map
        InstantiatedEvents.at(eventName)->stop(FMOD_STUDIO_STOP_MODE::FMOD_STUDIO_STOP_ALLOWFADEOUT);  
      }

    }

    /**************************************************************************/
    /*!
    \brief  Releases a sound from FMOD.
    */
    /**************************************************************************/
    void AudioFMOD::Unload(FMOD::Studio::Bank * bank)
    {
      bank->unload();
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
