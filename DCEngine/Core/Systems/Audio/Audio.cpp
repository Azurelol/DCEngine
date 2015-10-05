#include "Audio.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    Audio::Audio() : System(std::string("AudioSystem"), EnumeratedSystem::Audio) {
    #if(USE_FMOD)
      trace << "*Using FMOD for Audio \n";
      AudioHandler.reset(new AudioFMOD());
    #else
    #endif
    }

    void Audio::Register(SoundSpace & soundSpace)
    {
      SoundSpaceContainer.push_back(&soundSpace);
      trace << "Audio::Register -  " << soundSpace.Owner()->Name()
        << " has registered to the Audio system\n";
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

    void Audio::CreateSound(std::string & soundFile, FMODSoundPtr& soundPtr)
    {
      std::string resourceLocation("Core/Resources/Sounds/");
      AudioHandler->CreateSound(resourceLocation + soundFile, soundPtr.SoundPtr);
    }

    void Audio::PlaySound(std::string& soundCueName) {
      // Grab the 'SoundCue' resource from the content system
      auto soundCue = Daisy->getSystem<Content>()->getSoundCue(std::string(soundCueName));
      AudioHandler->PlaySound(soundCue->SoundPtr.SoundPtr, soundCue->Loop);

    }

    void Audio::StopSound(SoundCue & soundcue) {
    }
    
    void Audio::PlayMusic(std::string & filePath) {
      if (TRACE_ON)
        trace << "Audio::PlayMusic - Playing: " << filePath.c_str() << "\n";
      AudioHandler->PlayMusic(filePath);
    }

    void Audio::StopMusic() {
      AudioHandler->StopMusic();
    }

    void Audio::Terminate() {
      if (TRACE_ON)
        trace << "Audio::Terminate \n";
      AudioHandler->Terminate();
    }


  }
}