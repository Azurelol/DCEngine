/******************************************************************************/
/*!
@file   Audio.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/9/2015
@brief  The abstract interface for the Audio class.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "../System.h"

// Resources
#include "../../Components/SoundSpace.h"
#include "../../Resources/SoundCue.h"

// Switch for Audio Interface
#define USE_FMOD 1

// Audio Handler
#if(USE_FMOD)
#include "AudioFMOD.h"
class AudioFMOD;
#else
#endif

namespace DCEngine {
  class Engine;

  namespace Systems {
    class Audio : public System {
      friend class Engine;
      friend class SoundSpace;
    public:

      void Add(const std::string& bankFile, Systems::BankInfo& data);
      void CreateSound(const std::string& soundFile, FMODSoundHandle& soundPtr);
      SoundInstanceHandle PlaySound(const std::string& soundCueName);
      void PlaySoundOnce(const std::string& soundCueName);
      void ResumeSound(SoundInstance& instance);
      void ResumeSound(const std::string& soundCueName);
      void PauseSound(const std::string& soundCueName);
      void StopSound(const std::string& soundCueName);
      void Register(Components::SoundSpace& soundSpace);
      void Generate();

      ~Audio(); // @todo plz no

    private:

      bool Enabled = false;
      std::vector<Components::SoundSpace*> SoundSpaceContainer;
      SoundInstanceContainer ActiveInstances;

      SoundInstanceHandle CreateSoundInstance(const SoundCueHandle& soundCue);
      PlaybackSettings RetrievePlaybackSettings(const SoundCueHandle& soundCue);

      Audio();
      void Initialize();
      void Subscribe();
      void Update(float dt);
      void Terminate();
      void LoadSoundCues();
      void OnEnginePauseEvent(Events::EnginePause* event);
      void OnEngineResumeEvent(Events::EngineResume* event);


      #if(USE_FMOD)
      std::unique_ptr<AudioFMOD> AudioHandler;
      #else
      #endif



    };

  }
}