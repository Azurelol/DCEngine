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

    class AudioFMOD {   

    friend class Audio;
    public:
      // Interface
      void PlaySound(FMOD::Sound* soundPtr, FMOD::Channel** channel, bool bLoop = false);
      void ResumeSound(FMOD::Channel* channel);
      void PauseSound(FMOD::Channel* channel);
      void StopSound(FMOD::Channel* channel);
      void ReleaseSound(FMOD::Sound* soundPtr);  

      AudioFMOD();
      ~AudioFMOD();

    private:
      // Create
      void CreateSound(std::string& soundFile, FMOD::Sound** soundPtr);
      void CreateStream(std::string& soundFile, FMOD::Sound** soundPtr);      
      // Banks
      FMOD::Studio::Bank* LoadBankFromFile(std::string handle, std::string& path);
      FMOD::Studio::Bank* getBank(std::string handle);
      using BanksContainer = std::map<std::string, FMOD::Studio::Bank*>;
      BanksContainer ActiveBanks;
      // Channels
      using FMODChannelMap = std::map<FMOD::Sound*, FMOD::Channel*>;
      FMODChannelMap Channels;
      FMOD::Channel* CurrentChannel;
       // System
      void ErrorCheck(FMOD_RESULT result);
      void InitializeLowLevelAPI();
      void InitializeStudioAPI();
      void Initialize();
      void Update(float dt);
      void Terminate();
      FMODSystemPtr System;
      // Settings
      unsigned int MaxChannels;

    };


  }
}