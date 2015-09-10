#include "Audio.h"

namespace DCEngine {
  namespace Systems {

    Audio::Audio() : System(std::string("AudioSystem"), EnumeratedSystem::Audio) {
    #if(USE_FMOD)
      trace << "*Using FMOD for Audio \n";
      AudioHandler.reset(new AudioFMOD());
    #else
    #endif
    }

    Audio::~Audio() {
    }

    void Audio::PlaySound(SoundCue & soundcue) {


    }

    void Audio::StopSound(SoundCue & soundcue) {
    }

    void Audio::Initialize() {
      if (TRACE_ON)
        trace << "Audio::Initialize \n";
      AudioHandler->Initialize();
    }

    void Audio::Update(float dt) {
      if (TRACE_ON && TRACE_UPDATE)
        trace << "Audio::Update \n";
      AudioHandler->Update(dt);
    }

    void Audio::Terminate() {
      if (TRACE_ON)
        trace << "Audio::Terminate \n";
      AudioHandler->Terminate();
    }

    void Audio::PlaySound(std::string & fileName) {
      if (TRACE_ON)
        trace << "Audio::PlaySound - Playing: " << fileName.c_str() << "\n";

      FMOD::Sound* soundFile;

      std::string resourceLocation("Projects/Resources/Sounds/");
      
      AudioHandler->CreateSound(&soundFile, resourceLocation + fileName);
      AudioHandler->PlaySound(soundFile, true);
    }

  }
}