/******************************************************************************/
/*!
@file   Audio.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/9/2015
@brief  The abstract interface for the Audio class.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
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
      DCTrace << "*Using FMOD for Audio \n";
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
      DCTrace << "Audio::Register -  " << soundSpace.Owner()->Name()
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
        DCTrace << "Audio::Initialize \n";
      AudioHandler->Initialize();
      // Load every SoundCue into FMOD
      LoadSoundCues();
    }

    void Audio::Subscribe()
    {
      Daisy->Connect<Events::EnginePause>(&Audio::OnEnginePauseEvent, this);
      Daisy->Connect<Events::EngineResume>(&Audio::OnEngineResumeEvent, this);
    }

    void Audio::OnEnginePauseEvent(Events::EnginePause * event)
    {
      Enabled = false;
    }

    void Audio::OnEngineResumeEvent(Events::EngineResume * event)
    {
      Enabled = true;
    }

    /**************************************************************************/
    /*!
    @brief  Updates the Audio system. This is a necessary step for FMOD.
    @param  dt The delta time.
    */
    /**************************************************************************/
    void Audio::Update(float dt) {
      if (TRACE_ON && TRACE_UPDATE)
        DCTrace << "Audio::Update \n";
      if (Enabled)
        AudioHandler->Update(dt);
    }
    /**************************************************************************/
    /*!
    @brief  Registers a SoundFile to be played through FMOD.
    @param  soundFile The name of the sound file.
    @param  soundPtr  A pointer to the FMODSound pointer.
    @todo   Use different functions to create (sound/stream) depending on the
            file size.
    */
    /**************************************************************************/
    void Audio::CreateSound(std::string & soundFile, FMODSoundPtr& soundPtr)
    {
      //std::string resourceLocation("Core/Resources/Sounds/");

      //AudioHandler->CreateSound(soundFile, &soundPtr.SoundPtr);

      // 1. Check the size of the file on disk
      auto Kilo = 1000;
      auto soundFileSize = FileSystem::FileSize(soundFile) / Kilo;

      // 2.A If the file size was larger than 'x' (such as a music file)
      //     we will create the sound from a stream.
      if (soundFileSize > 1) {
        AudioHandler->CreateStream(soundFile, &soundPtr.SoundPtr);
      }
      // 2.B If the file size was not that big, load the whole sound file
      //     at once.
      else {
        AudioHandler->CreateSound(soundFile, &soundPtr.SoundPtr);
      }
      

    }

    /**************************************************************************/
    /*!
    @brief  Plays a sound cue.
    @param  soundCueName The name (string) of the sound in the content system.
    */
    /**************************************************************************/
    void Audio::PlaySound(std::string& soundCueName) {
      //DCTrace << "Audio::PlaySound - Playing SoundCue: " << soundCueName << "\n";
      auto soundCue = Daisy->getSystem<Content>()->getSoundCue(std::string(soundCueName));
      // Do not attempt to play if the soundcue could not be found
      if (!soundCue)
        return;

      AudioHandler->PlaySound(soundCue->Data.SoundPtr, &soundCue->Data.Channel, soundCue->Loop);
    }

    /**************************************************************************/
    /*!
    @brief  Resumes the playing of a sound.
    @param  soundCueName The name (string) of the sound in the content system.
    */
    /**************************************************************************/
    void Audio::ResumeSound(std::string & soundCueName)
    {
      DCTrace << "Audio::ResumeSound - Resuming SoundCue: " << soundCueName << "\n";
      auto soundCue = Daisy->getSystem<Content>()->getSoundCue(std::string(soundCueName));
      AudioHandler->ResumeSound(soundCue->Data.Channel);
    }

    /**************************************************************************/
    /*!
    @brief  Pauses a sound cue.
    @param  soundCueName The name (string) of the sound in the content system.
    */
    /**************************************************************************/
    void Audio::PauseSound(std::string & soundCueName)
    {
      DCTrace << "Audio::PlaySound - Playing SoundCue: " << soundCueName << "\n";
      auto soundCue = Daisy->getSystem<Content>()->getSoundCue(std::string(soundCueName));
      AudioHandler->PauseSound(soundCue->Data.Channel);
    }

    /**************************************************************************/
    /*!
    @brief  Stops a sound cue.
    @param  soundCueName The name (string) of the sound in the content system.
    */
    /**************************************************************************/
    void Audio::StopSound(std::string & soundCueName)
    {
      auto soundCue = Daisy->getSystem<Content>()->getSoundCue(std::string(soundCueName));
      AudioHandler->StopSound(soundCue->Data.Channel);
    }

    /**************************************************************************/
    /*!
    @brief  Stops a sound from playing through FMOD, by releasing it.
    @param  soundCueName The name (string) of the sound in the content system.
    */
    /**************************************************************************/
    //void Audio::ReleaseSound(std::string& soundCueName) {
    //  auto soundCue = Daisy->getSystem<Content>()->getSoundCue(std::string(soundCueName));
    //  AudioHandler->ReleaseSound(soundCue->Data.SoundPtr);
    //}

    /**************************************************************************/
    /*!
    @brief  Loads every SoundCue into memory.
    */
    /**************************************************************************/
    void Audio::LoadSoundCues()
    {
      if (TRACE_INITIALIZE)
        DCTrace << "Audio::LoadSoundCues - Loading every SoundCue into memory!";

      for (auto soundCue : *Daisy->getSystem<Content>()->AllSoundCues() ) {
        soundCue.second->Load();
      }
      DCTrace << "Audio::LoadSoundCues - Done loading!";
    }


    
    void Audio::Terminate() {
      if (TRACE_ON)
        DCTrace << "Audio::Terminate \n";
      AudioHandler->Terminate();
    }





  }
}