/******************************************************************************/
/*!
\file   Audio.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/9/2015
\brief  The abstract interface for the Audio class.

*/
/******************************************************************************/
#pragma once
#include "../System.h"

#include "../../Objects/Resources/SoundCue.h" // The SoundCue object

#define USE_FMOD 1

// Audio Handler
#if(USE_FMOD)
#include "AudioFMOD.h"
class AudioFMOD;
#else
#endif

namespace DCEngine {
  
  namespace Systems {   
    class Audio : public System {
      friend class Engine;
      friend class SoundSpace;
    public:
      Audio();
      ~Audio();

      void PlaySound(SoundCue& soundcue);
      void StopSound(SoundCue& soundcue);

      void Initialize();
      void Update(float dt);
      void Terminate();

    private:
      void PlaySound(std::string& filePath);
      void PlayMusic(std::string& filePath);

    #if(USE_FMOD)
    std::unique_ptr<AudioFMOD> AudioHandler;
    #else
    #endif



    };

  }
}