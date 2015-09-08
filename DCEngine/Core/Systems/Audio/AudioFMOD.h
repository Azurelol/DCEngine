

#pragma once
#include "..\..\..\Dependencies\include\FMOD\fmod.hpp"
#include "..\..\..\Dependencies\include\FMOD\fmod_errors.h"

#include <memory>

namespace DCEngine {
  namespace Systems {
    class AudioFMOD {
    public:
      AudioFMOD();
      ~AudioFMOD();

      void Initialize();
      void Update(float dt);
      void Terminate();

    private:
      //FMOD::System* _system;
      std::unique_ptr<FMOD::System> _system;


    };


  }
}