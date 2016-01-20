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
#include "FMODSystemPtr.h"
#include "..\..\..\Core\Debug\Debug.h"

namespace DCEngine {
  namespace Systems {

    // @todo Perhaps use this instead?
    struct AudioFMODSettings {
      unsigned MaxChannels;
      unsigned Volume;
      unsigned Pitch;
      bool Paused;
      float Level;
      bool Muted;

    };

    class AudioFMOD {   

    friend class Audio;
    public:
      // Interface
      void PlaySound(FMOD::Sound* soundPtr, FMOD::Channel** channel, bool bLoop = false);
      void ResumeSound(FMOD::Channel* channel);
      void PauseSound(FMOD::Channel* channel);
      void StopSound(FMOD::Channel* channel);
      void ReleaseSound(FMOD::Sound* soundPtr);  
      
      FMOD_RESULT Start();
      FMOD_RESULT Stop(FMOD_STUDIO_STOP_MODE mode);
      FMOD_RESULT Unload();
      FMOD_RESULT StopAllEvents(FMOD_STUDIO_STOP_MODE mode);
      // Properties
      DCE_DEFINE_PROPERTY(unsigned, Volume);
      DCE_DEFINE_PROPERTY(unsigned, Pitch);
      DCE_DEFINE_PROPERTY(bool, Paused);
      DCE_DEFINE_PROPERTY(float, Level);
      DCE_DEFINE_PROPERTY(bool, Muted);
      // Getters
        // Bank
        FMOD_RESULT getBank(std::string path, FMOD::Studio::Bank** bank);
        // Bus
        FMOD::Studio::Bus* getBus(std::string path);
        FMOD_RESULT getBus(std::string path, FMOD::Studio::Bus **bus) const;
        // VCA
        FMOD_RESULT getVCA(std::string path, FMOD::Studio::VCA** vca) const;
        FMOD::Studio::VCA* getVCA(std::string path) const;
        // Event
        FMOD_RESULT getEvent(std::string path, )




      AudioFMOD();
      ~AudioFMOD();

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

      // Create
      void CreateSound(std::string& soundFile, FMOD::Sound** soundPtr);
      void CreateStream(std::string& soundFile, FMOD::Sound** soundPtr);      
      // Channels
      using FMODChannelMap = std::map<FMOD::Sound*, FMOD::Channel*>;
      FMODChannelMap Channels;
      FMOD::Channel* CurrentChannel;
      // Banks
      FMOD::Studio::Bank* LoadBankFromFile(std::string handle, std::string& path);
      FMOD::Studio::Bank* getBank(std::string handle);
      using BanksContainer = std::map<std::string, FMOD::Studio::Bank*>;
      BanksContainer ActiveBanks;
      // Event Instance
      FMOD_RESULT CreateEventInstance(FMOD::Studio::EventInstance** instance) const;
      FMOD::Studio::EventInstance* CreateEventInstance() const;



      // System
      FMOD_RESULT getEvent(const char *path, FMOD::Studio::EventDescription **event) const;
      FMOD_RESULT getVCA(const char *path, FMOD::Studio::VCA **vca) const;
      FMOD_RESULT getBank(const char *path, FMOD::Studio::Bank **bank) const;


      void ErrorCheck(FMOD_RESULT result);
      void InitializeLowLevelAPI();
      void InitializeStudioAPI();
      void Initialize();
      void Update(float dt);
      void Terminate();


    };


  }
}