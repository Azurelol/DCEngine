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
*/
/******************************************************************************/
#pragma once

// Libraries
#include <memory>
#include <string>
#include <map>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>

// Headers
#include "FMODSystemPtr.h"
#include "..\..\..\Core\Debug\Debug.h"

namespace DCEngine {
  namespace Systems {

    class AudioFMOD {

      using FMODChannelMap = std::map<FMOD::Sound*, FMOD::Channel*>;

    friend class Audio;
    public:
      AudioFMOD();
      ~AudioFMOD();

      void PlaySound(FMOD::Sound* soundPtr, FMOD::Channel** channel, bool bLoop = false);
      void ResumeSound(FMOD::Channel* channel);
      void PauseSound(FMOD::Channel* channel);
      void StopSound(FMOD::Channel* channel);
      //void PlaySound(FMOD::Sound* soundPtr, bool bLoop = false);
      //void StopSound(FMOD::Sound* channel);
      void ReleaseSound(FMOD::Sound* soundPtr);

      void Initialize();
      void Update(float dt);
      void Terminate();

    private:
      void ErrorCheck(FMOD_RESULT result);
      void CreateSound(std::string& soundFile, FMOD::Sound** soundPtr);
      void CreateStream(std::string& soundFile, FMOD::Sound** soundPtr);

      FMODSystemPtr system_; // Wrapper for the C-style pointer
      FMOD_RESULT OperationResult;
      unsigned int MaxChannels;
      FMOD::Sound* MusicPtr;
      FMODChannelMap Channels; //!< Every SoundPtr gets its own unique channel?
      FMOD::Channel* CurrentChannel;


    };


  }
}