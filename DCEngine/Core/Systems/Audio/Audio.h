/******************************************************************************/
/*!
@file   Audio.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/9/2015
@brief  The abstract interface for the Audio class.

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
      void StopSound(std::string& soundcueName);

      void Register(SoundSpace& soundSpace);

    private:

      std::vector<SoundSpace*> SoundSpaceContainer; //!< Container of registered soundspace components.

      Audio();
      void Initialize();
      void Update(float dt);
      void Terminate();

      void PlayMusic(std::string& filePath); //!< Starts playing from a music file.
      void StopMusic(); //!< Stops the currently playing music file.

      #if(USE_FMOD)
      std::unique_ptr<AudioFMOD> AudioHandler;
      #else
      #endif



    };

  }
}