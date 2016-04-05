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
    Audio::Audio(AudioConfig& config) : System(std::string("AudioSystem"), EnumeratedSystem::Audio), Settings(config) {
      #if(USE_FMOD)
      DCTrace << "*Using FMOD for Audio \n";
      AudioHandler.reset(new AudioFMOD());
      #else
      #endif
    }

    /**************************************************************************/
    /*!
    @brief  Audio system destructor.
    */
    /**************************************************************************/
    Audio::~Audio()
    {
      // Now that the Audio system is being destroyed, prevent sound instances
      // from attempting to release sounds.
      SoundInstance::ReleaseOnDestroyed = false;
    }

    /**************************************************************************/
    /*!
    @brief  Registers a space into the Audio system.
    @param  soundSpace The 'SoundSpace' component of the space.
    */
    /**************************************************************************/
    void Audio::Register(Components::SoundSpace & soundSpace)
    {
      SoundSpaceContainer.push_back(&soundSpace);
      DCTrace << "Audio::Register -  " << soundSpace.Owner()->Name()
        << " has registered to the Audio system\n";
    }

    /**************************************************************************/
    /*!
    @brief  Generates audio resources from all currently loaded banks.
    */
    /**************************************************************************/
    void Audio::Generate()
    {
      DCTrace << "Audio::Generate: Generating audio resources from all loaded banks! \n";
      AudioHandler->GenerateResources();
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
      Subscribe();
    }

    /**************************************************************************/
    /*!
    @brief  Subscribes to events.
    */
    /**************************************************************************/
    void Audio::Subscribe()
    {
      Daisy->Connect<Events::EnginePause>(&Audio::OnEnginePauseEvent, this);
      Daisy->Connect<Events::EngineResume>(&Audio::OnEngineResumeEvent, this);
    }

    void Audio::OnEnginePauseEvent(Events::EnginePause * event)
    {
      DCTrace << "Audio::OnEnginePauseEvent - Pause \n";
      Settings.Enabled = false;
    }

    void Audio::OnEngineResumeEvent(Events::EngineResume * event)
    {
      DCTrace << "Audio::OnEngineResumeEvent - Pause \n";
      Settings.Enabled = true;
    }

    /**************************************************************************/
    /*!
    @brief  Updates the Audio system. This is a necessary step for FMOD.
    @param  dt The delta time.
    */
    /**************************************************************************/
    void Audio::Update(float dt) {
      SystemTimer profile(this->Name());
      if (TRACE_ON && TRACE_UPDATE)
        DCTrace << "Audio::Update \n";
      if (Settings.Enabled)
        AudioHandler->Update(dt);    

    }

    /**************************************************************************/
    /*!
    @brief  Adds an audio Bank to the audio system.
    @param  bankFile The name of the bank file.
    */
    /**************************************************************************/
    void Audio::Add(const std::string & bankFile, Systems::BankInfo& data)
    {
      // Adds the bank to the FMOD Studio system
      data = AudioHandler->LoadBankFromFile(FileSystem::FileNoExtension(bankFile), bankFile);
    }

    /**************************************************************************/
    /*!
    @brief  Creates a SoundInstance off a given SoundCue.
    @param  The name of the SoundInstance.
    @return A SoundInstance.
    */
    /**************************************************************************/
    SoundInstancePtr Audio::CreateSoundInstance(const SoundCueHandle & soundCueName)
    {
      // Pulls the data from the SoundCue and save it on the instance.
      auto soundCue = Daisy->getSystem<Content>()->getSoundCue(std::string(soundCueName));
      return CreateSoundInstance(soundCue);

      //SoundInstancePtr instance(new SoundInstance());

      //instance->Type = soundCue->Type;      
      //// Copy the Playback settings
      //instance->Settings.Mode = soundCue->Mode;
      //instance->Settings.Volume = soundCue->Volume;
      //instance->Settings.VolumeVariation = soundCue->VolumeVariation;
      //instance->Settings.Pitch = soundCue->Pitch;
      //instance->Settings.PitchVariation = soundCue->PitchVariation;      
      //// Copy different things depending on what type of SoundCue we are instantiating from:
      //if (soundCue->Type == SoundCue::SoundCueType::Event)
      //  instance->StudioEventName = soundCue->Name();
      //else if (soundCue->Type == SoundCue::SoundCueType::File)
      //  instance->SoundHandle.Handle = soundCue->Data.Handle; 

      ///* NOTE: For LL, we only copy the handle to the underlying FMOD::Sound* since we want
      //         to be using an unique channel to the instance. */
      //return instance;
    }

    /**************************************************************************/
    /*!
    @brief  Creates a SoundInstance off a given SoundCue.
    @param  soundCue The name of the SoundCue.
    @return A SoundInstanceHandle
    */
    /**************************************************************************/
    SoundInstanceHandle Audio::CreateSoundInstanceHandle(const SoundCueHandle & soundCueName)
    {
      // Pulls the data from the SoundCue and save it on the instance.
      auto soundCue = Daisy->getSystem<Content>()->getSoundCue(std::string(soundCueName));
      // Allocate the SoundInstance through Zilch
      auto state = Daisy->getSystem<Reflection>()->Handler()->GetState();
      Zilch::ExceptionReport report;
      Zilch::Handle instanceHandle = state->AllocateDefaultConstructedHeapObject(ZilchTypeId(SoundInstance), 
                                                               report, Zilch::HeapFlags::ReferenceCounted);
      // Dereference it so we can use it
      auto instance = reinterpret_cast<SoundInstance*>(instanceHandle.Dereference());
      instance->setObjectName(soundCueName);
      // Pulls the data from the SoundCue and save it on the instance.
      instance->Type = soundCue->Type;
      // Copy the Playback settings
      instance->Settings.Mode = soundCue->Mode;
      instance->Settings.Volume = soundCue->Volume;
      instance->Settings.VolumeVariation = soundCue->VolumeVariation;
      instance->Settings.Pitch = soundCue->Pitch;
      instance->Settings.PitchVariation = soundCue->PitchVariation;
      // Copy different things depending on what type of SoundCue we are instantiating from:
      if (soundCue->Type == SoundCue::SoundCueType::Event)
        instance->StudioEventName = soundCue->Name();
      else if (soundCue->Type == SoundCue::SoundCueType::File)
        instance->SoundHandle.Handle = soundCue->Data.Handle;

      return instanceHandle;
    }

    /**************************************************************************/
    /*!
    @brief  Creates a SoundInstance off a given SoundCue.
    @param  A handle to the SoundCue.
    @return A SoundInstance.
    */
    /**************************************************************************/
    SoundInstancePtr Audio::CreateSoundInstance(const SoundCuePtr & soundCue)
    {
      SoundInstancePtr instance(new SoundInstance());

      // Pulls the data from the SoundCue and save it on the instance.
      instance->Type = soundCue->Type;
      // Copy the Playback settings
      instance->Settings.Mode = soundCue->Mode;
      instance->Settings.Volume = soundCue->Volume;
      instance->Settings.VolumeVariation = soundCue->VolumeVariation;
      instance->Settings.Pitch = soundCue->Pitch;
      instance->Settings.PitchVariation = soundCue->PitchVariation;
      // Copy different things depending on what type of SoundCue we are instantiating from:
      if (soundCue->Type == SoundCue::SoundCueType::Event)
        instance->StudioEventName = soundCue->Name();
      else if (soundCue->Type == SoundCue::SoundCueType::File)
        instance->SoundHandle.Handle = soundCue->Data.Handle;

      /* NOTE: For LL, we only copy the handle to the underlying FMOD::Sound* since we want
      to be using an unique channel to the instance. */
      return instance;
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
    void Audio::CreateSound(const std::string & soundFile, FMODSoundHandle& soundPtr)
    {
      // 1. Check the size of the file on disk
      auto Kilo = 1000;
      auto soundFileSize = FileSystem::FileSize(soundFile) / Kilo;

      // 2.A If the file size was larger than 'x' (such as a music file)
      //     we will create the sound from a stream.
      if (soundFileSize > 1) {
        AudioHandler->CreateStream(soundFile, &soundPtr.Handle);
      }
      // 2.B If the file size was not that big, load the whole sound file
      //     at once.
      else {
        AudioHandler->CreateSound(soundFile, &soundPtr.Handle);
      }
      

    }

    /**************************************************************************/
    /*!
    @brief  Plays a sound cue.
    @param  soundCueName The name (string) of the sound in the content system.
    @return A SoundInstancePtr.
    */
    /**************************************************************************/
    SoundInstancePtr Audio::PlaySound(const std::string& soundCueName) {

      DCTrace << "Audio::PlaySound - Playing SoundCue: " << soundCueName << "\n";
      auto soundCue = Daisy->getSystem<Content>()->getSoundCue(std::string(soundCueName));
      if (!soundCue)
        return nullptr;

      return PlaySound(soundCue);
    }

    /**************************************************************************/
    /*!
    @brief  Plays a sound cue.
    @param  soundCueName The name (string) of the sound in the content system.
    @return A SoundInstancePtr.
    */
    /**************************************************************************/
    SoundInstancePtr Audio::PlaySound(SoundCuePtr cue)
    {
      // Do not attempt to play if the soundcue could not be found
      if (!cue) {
        DCTrace << "Audio::PlaySound - Could not find: " << cue->Name() << "\n";
        return nullptr;
      }
      // Create an unique SoundInstance for this SoundCue
      SoundInstancePtr instance = CreateSoundInstance(cue);
      // Depending on the type of SoundCue, play it through the low level API
      // or as an event belonging to the Studip API
      if (cue->Type == SoundCue::SoundCueType::File)
        AudioHandler->PlaySound(instance->SoundHandle.Handle, &(instance->SoundHandle.Channel), instance->Settings);
      else if (cue->Type == SoundCue::SoundCueType::Event)
        AudioHandler->PlaySound(instance->StudioEventName, &(instance->SoundHandle.EventInstance), instance->Settings);

      // Return a handle to the SoundInstance
      return instance;
    }

    /**************************************************************************/
    /*!
    @brief  Plays a sound cue, returning a Zilch Handle.
    @param  name The name (string) of the sound in the content system.
    @return A SoundInstanceHandle.
    */
    /**************************************************************************/
    SoundInstanceHandle Audio::PlaySoundZilch(const std::string & name)
    {
      DCTrace << "Audio::PlaySoundZilch - Playing SoundCue: " << name << "\n";
      auto soundCue = Daisy->getSystem<Content>()->getSoundCue(std::string(name));
      if (!soundCue)
        return Zilch::Handle();

      // Create an unique SoundInstance for this SoundCue through Zilch's MM
      SoundInstanceHandle instanceHandle = CreateSoundInstanceHandle(name);
      auto instance = SoundInstance::Dereference(instanceHandle);
      // Depending on the type of SoundCue, play it through the low level API
      // or as an event belonging to the Studip API
      if (instance->Type == SoundCue::SoundCueType::File)
        AudioHandler->PlaySound(instance->SoundHandle.Handle, &(instance->SoundHandle.Channel), instance->Settings);
      else if (instance->Type == SoundCue::SoundCueType::Event)
        AudioHandler->PlaySound(instance->StudioEventName, &(instance->SoundHandle.EventInstance), instance->Settings);

      // Return a handle to the SoundInstance
      return instanceHandle;
    }

    /**************************************************************************/
    /*!
    @brief  Resumes the playing of a sound.
    @param  soundCueName The name (string) of the sound in the content system.
    */
    /**************************************************************************/
    void Audio::ResumeSound(SoundInstance& instance)
    {
      
    }

    /**************************************************************************/
    /*!
    @brief  Resumes the playing of a sound.
    @param  soundCueName The name (string) of the sound in the content system.
    */
    /**************************************************************************/
    void Audio::ResumeSound(const std::string & soundCueName)
    {
      DCTrace << "Audio::ResumeSound - Resuming SoundCue: " << soundCueName << "\n";
      auto soundCue = Daisy->getSystem<Content>()->getSoundCue(std::string(soundCueName));
      if (!soundCue) {
        DCTrace << "Audio::PlaySound - Could not find: " << soundCueName << "\n";
        return;
      }
      AudioHandler->ResumeSound(soundCue->Data.Channel);
    }

    /**************************************************************************/
    /*!
    @brief  Pauses a sound cue.
    @param  soundCueName The name (string) of the sound in the content system.
    */
    /**************************************************************************/
    void Audio::PauseSound(const std::string & soundCueName)
    {
      DCTrace << "Audio::PlaySound - Pausing SoundCue: " << soundCueName << "\n";
      auto soundCue = Daisy->getSystem<Content>()->getSoundCue(std::string(soundCueName));
      if (!soundCue) {
        DCTrace << "Audio::PlaySound - Could not find: " << soundCueName << "\n";
        return;
      }
      AudioHandler->PauseSound(soundCue->Data.Channel);
    }

    /**************************************************************************/
    /*!
    @brief  Stops a sound cue.
    @param  soundCueName The name (string) of the sound in the content system.
    */
    /**************************************************************************/
    void Audio::StopSound(const std::string & soundCueName)
    {
      auto soundCue = Daisy->getSystem<Content>()->getSoundCue(std::string(soundCueName));
      if (!soundCue) {
        DCTrace << "Audio::PlaySound - Could not find: " << soundCueName << "\n";
        return;
      }

      if (soundCue->Type == SoundCue::SoundCueType::File)
        AudioHandler->StopSound(soundCue->Data.Channel);
      else if (soundCue->Type == SoundCue::SoundCueType::Event)
        AudioHandler->StopSound(soundCue->Data.EventInstance);      
    }

    /**************************************************************************/
    /*!
    @brief  Stops a sound from playing through FMOD, by releasing it.
    @param  soundCueName The name (string) of the sound in the content system.
    */
    /**************************************************************************/
    //void Audio::ReleaseSound(const std::string& soundCueName) {
    //  auto soundCue = Daisy->getSystem<Content>()->getSoundCue(const std::string(soundCueName));
    //  AudioHandler->ReleaseSound(soundCue->Data.Handle);
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

    /**************************************************************************/
    /*!
    @brief  Terminates the Audio system.
    */
    /**************************************************************************/    
    void Audio::Terminate() {
      if (TRACE_ON)
        DCTrace << "Audio::Terminate \n";
      AudioHandler->Terminate();
    }





  }
}