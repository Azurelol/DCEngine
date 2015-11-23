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

      void CreateSound(std::string& soundFile, FMODSoundPtr& soundPtr);
      void PlaySound(std::string& soundCueName);
      void ResumeSound(std::string& soundCueName);
      void PauseSound(std::string& soundCueName);
      void StopSound(std::string& soundCueName);
      void Register(SoundSpace& soundSpace);

    private:

      std::vector<SoundSpace*> SoundSpaceContainer; //!< Container of registered soundspace components.

      Audio();
      void Initialize();
      void Update(float dt);
      void Terminate();
      void LoadSoundCues();


      #if(USE_FMOD)
      std::unique_ptr<AudioFMOD> AudioHandler;
      #else
      #endif



    };

  }
}