
#pragma once
#include "..\..\..\Dependencies\include\FMOD\fmod.hpp"
#include "..\..\..\Dependencies\include\FMOD\fmod_errors.h"

#include <memory>
#include <string>
#include "..\..\..\Core\Debug\Debug.h"

namespace DCEngine {
  namespace Systems {
    
    class AudioFMOD {
      // Reference: "http://stackoverflow.com/questions/27680981/using-c11-smart-pointers-as-c-function-arguments"
      class FMODSystemPtr {
      public:
        FMOD::System* ptr;
        FMODSystemPtr() {
          auto result = FMOD::System_Create(&ptr);
          if (result != FMOD_OK) {
            ptr = nullptr;
            //throw something;
          }
          trace << "FMODSystemPtr - Constructor\n";
        }
        FMODSystemPtr(FMODSystemPtr&&) = default;
        FMODSystemPtr& operator=(FMODSystemPtr&&) = default;
        FMODSystemPtr(const FMODSystemPtr&) = delete;
        FMODSystemPtr& operator=(const FMODSystemPtr&) = delete;
        ~FMODSystemPtr() {
          auto result = ptr->release();
          //assert(result == FMOD_OK);  // destructors shouldn't throw: use your best judgment here
        }
      }; // FMODSystemPtr

    friend class Audio;
    public:
      AudioFMOD();
      ~AudioFMOD();



      void PlaySound(FMOD::Sound* soundPtr, bool bLoop = false);
      void PlayMusic(std::string& filePath);
      void StopMusic();
      void ReleaseSound(FMOD::Sound* soundPtr);

      void Initialize();
      void Update(float dt);
      void Terminate();

    private:
      void ErrorCheck(FMOD_RESULT result);
      void CreateSound(FMOD::Sound** soundPtr, std::string& filePtr);
      void CreateStream(FMOD::Sound** soundPtr, std::string& filePtr);

      //FMOD::System* _system;
      FMODSystemPtr system_; // Wrapper for the C-style pointer
      //FMOD::System* _system;
      FMOD::Sound* MusicPtr;
      FMOD::Channel* CurrentChannel;


    };


  }
}