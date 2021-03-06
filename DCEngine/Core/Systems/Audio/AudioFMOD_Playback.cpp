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
      // Have it release when its done playing
      handle->release();
      auto check = ErrorCheck(System.LowLevel->playSound(handle, NULL, 0, channel));
      (*channel)->setVolume(settings.Volume);
      (*channel)->setPitch(settings.Pitch);      
      return check;
    }

    /**************************************************************************/
    /*!
    @brief Plays a sound through FMOD Studio API once.
    @param eventDescription The name of the event which to play.
    @param settings A struct containing playback settings.
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
          auto eventInstance = CreateEventInstance(AvailableEvents.at(eventName));
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
    @brief Plays a sound through FMOD Studio API.
    @param event A pointer to the instance pointer.
    @param settings A struct containing playback settings.
    */
    /**************************************************************************/
    bool AudioFMOD::PlaySound(EventDescriptionHandle& eventHandle, FMOD::Studio::EventInstance ** instance, PlaybackSettings & settings)
    {
      // If the event is found in the map of available events, create it
      if (AvailableEvents.count(eventHandle)) {
        *instance = CreateEventInstance(AvailableEvents.at(eventHandle));
        // Configure it
        ErrorCheck((*instance)->setVolume(settings.Volume));
        ErrorCheck((*instance)->setPitch(settings.Pitch));
        // One-shot sound
        ErrorCheck((*instance)->start());
        return true;
      }
      return false;
    }

    /**************************************************************************/
    /*!
    @brief Resumes the playing of a sound through FMOD Low Level.
    @param channel A pointer to the channel.
    */
    /**************************************************************************/
    void AudioFMOD::ResumeSound(FMOD::Channel * channel)
    {
      ErrorCheck(channel->setPaused(false));
    }

    /**************************************************************************/
    /*!
    @brief Resumes the playing of a sound through FMOD Studio.
    @param eventHandle A reference to the name of the event.
    */
    /**************************************************************************/
    void AudioFMOD::ResumeSound(EventDescriptionHandle & eventHandle)
    {      
    }

    /**************************************************************************/
    /*!
    @brief  Resumes the playing of a sound through FMOD Studio.
    @param eventInstance A reference to the EventInstance object.
    */
    /**************************************************************************/
    void AudioFMOD::ResumeSound(FMOD::Studio::EventInstance & eventInstance)
    {
      eventInstance.setPaused(false);
    }

    /**************************************************************************/
    /*!
    @brief Stops a sound from playing through FMOD.
    @param channel A pointer to the channel.
    */
    /**************************************************************************/
    void AudioFMOD::PauseSound(FMOD::Channel* channel)
    {
      ErrorCheck(channel->setPaused(true));
    }

    /**************************************************************************/
    /*!
    @brief Stops a sound from playing through FMOD.
    @param eventInstance A reference to the event instance.
    */
    /**************************************************************************/
    void AudioFMOD::PauseSound(FMOD::Studio::EventInstance & eventInstance)
    {
      eventInstance.setPaused(true);
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
    @brief  Stops a sound from playing through FMOD Studio's EventInstance.
    */
    /**************************************************************************/
    void AudioFMOD::StopSound(FMOD::Studio::EventInstance * instance)
    {
      ErrorCheck(instance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT));
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
