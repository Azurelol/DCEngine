
#pragma once
#include "..\..\..\Dependencies\include\FMOD\fmod.hpp"
#include "..\..\..\Dependencies\include\FMOD\fmod_errors.h"

#include <memory>
#include <string>

namespace DCEngine {
  namespace Systems {
    class AudioFMOD {
    public:
      AudioFMOD();
      ~AudioFMOD();

      void CreateSound(FMOD::Sound** soundPtr, std::string& filePtr);
      void PlaySound(FMOD::Sound* soundPtr, bool bLoop = false);
      void PlayMusic(std::string& filePath);
      void ReleaseSound(FMOD::Sound* soundPtr);

      void Initialize();
      void Update(float dt);
      void Terminate();

    private:
      //FMOD::System* _system;
      std::unique_ptr<FMOD::System> system_;
      std::unique_ptr<FMOD::Channel> channel_;
      std::unique_ptr<FMOD::Sound> CurrentMusic;


    };


  }
}