#include "../System.h"
#pragma once

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
    public:
      Audio();
      ~Audio();

      void Initialize();
      void Update(float dt);
      void Terminate();

    private:
    #if(USE_FMOD)
    std::unique_ptr<AudioFMOD> AudioHandler;
    #else
    #endif



    };

  }
}