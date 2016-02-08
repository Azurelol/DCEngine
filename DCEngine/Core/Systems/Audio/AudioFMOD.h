/******************************************************************************/
/*!
@file   AudioFMOD.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/16/2015
@brief  The interface for FMOD.
@note   References:
        https://katyscode.wordpress.com/2012/10/05/cutting-your-teeth-on-fmod-
        part-1-build-environment-initialization-and-playing-sounds/
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

// Libraries
#include <memory>
#include <string>
#include <map>
#include <FMOD/fmod_studio.hpp>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>

// Headers
#include "AudioFMOD_Utilities.h"
#include "..\..\..\Core\Debug\Debug.h"

namespace DCEngine {
  namespace Systems {
    
    class AudioFMOD {   

    friend class Audio;
    public:
      AudioFMOD();
      ~AudioFMOD();

      // Playback
      bool PlaySound(FMOD::Sound* soundPtr, FMOD::Channel** channel, PlaybackSettings& settings);
      bool PlaySound(EventDescriptionHandle& eventHandle, PlaybackSettings& settings);
      void ResumeSound(FMOD::Channel* channel);
      void ResumeSound(EventDescriptionHandle& eventHandle);
      void PauseSound(FMOD::Channel* channel);
      void PauseSound(EventDescriptionHandle& eventHandle);
      void StopSound(FMOD::Channel* channel);
      void StopSound(EventDescriptionHandle& eventHandle);
      void StopAll();
      void SetVolume(FMOD::Channel* soundPtr, float volume);
      void SetVolume(EventDescriptionHandle& eventHandle, float volume);     

      // Accesors
      FMOD::Studio::Bank* getBank(const std::string handle);
      FMOD_RESULT getBank(const std::string path, FMOD::Studio::Bank** bank);
      FMOD::Studio::Bus* getBus(const std::string path);
      FMOD_RESULT getBus(const std::string path, FMOD::Studio::Bus **bus) const;
      FMOD::Studio::VCA* getVCA(const std::string path) const;
      FMOD_RESULT getVCA(const std::string path, FMOD::Studio::VCA** vca) const;
      
      // Generate
      void GenerateResources();
      void GenerateSoundCues();

      //FMOD_RESULT Start();
      //FMOD_RESULT Stop(FMOD_STUDIO_STOP_MODE mode);
      //FMOD_RESULT Unload();
      //FMOD_RESULT StopAllEvents(FMOD_STUDIO_STOP_MODE mode);     

    private:

      // Settings
      FMODSystemPtr System;
      AudioFMODSettings Settings;
      unsigned MaxChannels;
      unsigned Volume;
      unsigned Pitch;
      bool Paused;
      float Level;
      bool Muted;

      // Containers
      BanksContainer ActiveBanks;
      EventInstanceMap InstantiatedEvents;
      EventDescriptionMap AvailableEvents;
      GroupMap Groups;
      ChannelMap Channels;

      // Create
      bool CreateSound(const std::string& soundFile, FMOD::Sound** soundPtr);
      bool CreateSound(const std::string& eventDescrption);
      bool CreateStream(const std::string& soundFile, FMOD::Sound** soundPtr);      
      FMOD_RESULT CreateEventInstance(FMOD::Studio::EventInstance** instance) const;
      FMOD::Studio::EventInstance* AddEventInstance(FMOD::Studio::EventDescription* event) const;

      // Loading
      FMOD::Studio::Bank* LoadBankFromFile(const std::string handle, const std::string& path);      
      void LoadEventDescriptions(FMOD::Studio::Bank* bank);
      void LoadVCAs(FMOD::Studio::Bank* bank);
      void LoadChannelGroups(FMOD::Studio::Bank* bank);

      // Release
      void Unload(FMOD::Studio::Bank* bank);
      void ReleaseSound(FMOD::Sound* soundPtr);       

      // Accesors
      FMOD_RESULT getEvent(const char *path, FMOD::Studio::EventDescription **event) const;
      FMOD_RESULT getVCA(const char *path, FMOD::Studio::VCA **vca) const;
      FMOD_RESULT getBank(const char *path, FMOD::Studio::Bank **bank) const;

      // System
      bool ErrorCheck(FMOD_RESULT result);
      void InitializeLowLevelAPI();
      void InitializeStudioAPI();
      void Initialize();
      void Update(float dt);
      void Terminate();

      // Diagnostics
      static unsigned EventInstancesCreated;
      
    };


  }
}