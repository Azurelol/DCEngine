#include "Audio.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Constructs the Audio system.
    */
    /**************************************************************************/
    Audio::Audio() : System(std::string("AudioSystem"), EnumeratedSystem::Audio) {
      #if(USE_FMOD)
      trace << "*Using FMOD for Audio \n";
      AudioHandler.reset(new AudioFMOD());
      #else
      #endif
    }

    /**************************************************************************/
    /*!
    @brief  Registers a space into the Audio system.
    @param  soundSpace The 'SoundSpace' component of the space.
    */
    /**************************************************************************/
    void Audio::Register(SoundSpace & soundSpace)
    {
      SoundSpaceContainer.push_back(&soundSpace);
      trace << "Audio::Register -  " << soundSpace.Owner()->Name()
        << " has registered to the Audio system\n";
    }

    /**************************************************************************/
    /*!
    @brief  Registers a space into the Audio system.
    @param  soundSpace The 'SoundSpace' component of the space.
    */
    /**************************************************************************/
    void Audio::Initialize() {
      if (TRACE_ON)
        trace << "Audio::Initialize \n";
      AudioHandler->Initialize();
      // Load every SoundCue into FMOD
      LoadSoundCues();
    }

    /**************************************************************************/
    /*!
    @brief  Updates the Audio system. This is a necessary step for FMOD.
    @param  dt The delta time.
    */
    /**************************************************************************/
    void Audio::Update(float dt) {
      if (TRACE_ON && TRACE_UPDATE)
        trace << "Audio::Update \n";
      AudioHandler->Update(dt);
    }
    /**************************************************************************/
    /*!
    @brief  Registers a SoundFile to be played through FMOD.
    @param  soundFile The name of the sound file.
    @param  soundPtr  A pointer to the ..
    */
    /**************************************************************************/
    void Audio::CreateSound(std::string & soundFile, FMODSoundPtr& soundPtr)
    {
      //std::string resourceLocation("Core/Resources/Sounds/");
      AudioHandler->CreateSound(soundFile, &soundPtr.SoundPtr);
    }

    /**************************************************************************/
    /*!
    @brief  Plays a sound through FMOD.
    @param  soundCueName The name (string) of the sound in the content system.
    */
    /**************************************************************************/
    void Audio::PlaySound(std::string& soundCueName) {
      auto soundCue = Daisy->getSystem<Content>()->getSoundCue(std::string(soundCueName));
      AudioHandler->PlaySound(soundCue->SoundPtr.SoundPtr, soundCue->Loop);
    }

    /**************************************************************************/
    /*!
    @brief  Stops a sound from playing through FMOD, by releasing it.
    @param  soundCueName The name (string) of the sound in the content system.
    */
    /**************************************************************************/
    void Audio::ReleaseSound(std::string& soundCueName) {
      auto soundCue = Daisy->getSystem<Content>()->getSoundCue(std::string(soundCueName));
      AudioHandler->ReleaseSound(soundCue->SoundPtr.SoundPtr);
    }

    /**************************************************************************/
    /*!
    @brief  Loads every SoundCue into memory.
    */
    /**************************************************************************/
    void Audio::LoadSoundCues()
    {
      if (TRACE_INITIALIZE)
        trace << "Audio::LoadSoundCues - Loading every SoundCue into memory!";

      for (auto soundCue : *Daisy->getSystem<Content>()->AllSoundCues() ) {
        soundCue.second->Load();
      }
      trace << "Audio::LoadSoundCues - Done loading!";
    }


    /* Music Testing */
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

    void Audio::Serialize(Json::Value & root)
    {
    }

    void Audio::Deserialize(Json::Value & root)
    {
    }




  }
}