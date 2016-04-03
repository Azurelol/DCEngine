/*****************************************************************************/
/*!
@file      SoundInstance.cpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      2/9/2015
@brief     SoundInstance is an object created whenever a SoundCue is played that
           acts as the particular instance of that playing of the SoundCue as well
           as a direct representation of the sound currently playing.
           The SoundInstance object is only created through code: through
           the SoundEmitter or SoundSpace.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "SoundInstance.h"

// Engine
#include "../../Core/Engine/Engine.h"

namespace DCEngine {

  unsigned SoundInstance::Created = 0;
  unsigned SoundInstance::Destroyed = 0;
  bool SoundInstance::ReleaseOnDestroyed = true;
  bool SoundInstance::StopOnDestroyed = false;

  /*!************************************************************************\
  @brief  Zilch Component Definition
  \**************************************************************************/
  ZilchDefineType(SoundInstance, "SoundInstance", DCEngineCore, builder, type) {
    //DCE_BINDING_SET_HANDLE_TYPE_POINTER;
    // Constructor / Destructor    
    ZilchBindConstructor(builder, type, SoundInstance, ZilchNoNames);
    ZilchBindDestructor(builder, type, SoundInstance);
    // Methods
    ZilchBindMethod(builder, type, &SoundInstance::SetParameter, ZilchNoOverload, "SetParameter", "parameter, value");
    ZilchBindMethod(builder, type, &SoundInstance::InterpolateVolume, ZilchNoOverload, "InterpolateVolume", "newVolume, time");
    ZilchBindMethod(builder, type, &SoundInstance::InterpolatePitch, ZilchNoOverload, "InterpolatePitch", "newPitch, time");
    ZilchBindMethod(builder, type, &SoundInstance::Resume, ZilchNoOverload, "Resume", ZilchNoNames);
    ZilchBindMethod(builder, type, &SoundInstance::Pause, ZilchNoOverload, "Pause", ZilchNoNames);
    ZilchBindMethod(builder, type, &SoundInstance::Stop, ZilchNoOverload, "Stop", ZilchNoNames);
    ZilchBindMethod(builder, type, &SoundInstance::IsPlaying, ZilchNoOverload, "IsPlaying", ZilchNoNames);
    // Properties
  }

  /**************************************************************************/
  /*!
  @brief  SoundInstance constructor.
  */
  /**************************************************************************/
  SoundInstance::SoundInstance() : Paused(false), SoundInstanceID(Created++)
  {
  }

  /**************************************************************************/
  /*!
  @brief  SoundInstance destructor.
  */
  /**************************************************************************/
  SoundInstance::~SoundInstance()
  {
    // Switched on and off when transitioning between Play into Editor mode
    if (StopOnDestroyed)
      this->Stop();    

    // If the engine's audio system is not active, do not attempt to release.
    // Reference: http://www.fmod.org/questions/question/forum-40474/
    if (!ReleaseOnDestroyed)
      return;

    // Studio
    if (SoundHandle.EventInstance)
      SoundHandle.EventInstance->release();
    // Low Level: We don't release the sound pointer!!!
    //if (SoundHandle.Channel)
    //  SoundHandle.Channel->
  }

  /**************************************************************************/
  /*!
  @brief A function that interpolates the volume of a SoundInstance to a new 
         value over a certain amount of time.
  @param newVolume The new volume one wishes to interpolate to.
  @param time The seconds over which the new volume will be interpolated.
  */
  /**************************************************************************/
  void SoundInstance::InterpolateVolume(Real newVolume, Real time)
  {
    Settings.Volume = newVolume;
    if (Type == SoundCue::SoundCueType::Event && SoundHandle.EventInstance)
      SoundHandle.EventInstance->setVolume(newVolume);
    else if (Type == SoundCue::SoundCueType::File && SoundHandle.Channel)
      SoundHandle.Channel->setVolume(newVolume);
  }

  /**************************************************************************/
  /*!
  @brief A function that interpolates the pitch of a SoundInstance to a new
          value over a certain amount of time.
  @param newPitch The new pitch one wishes to interpolate to.
  @param time The seconds over which the new volume will be interpolated.
  */
  /**************************************************************************/
  void SoundInstance::InterpolatePitch(Real newPitch, Real time)
  {

  }

  /**************************************************************************/
  /*!
  @brief  A function that resumes the paused SoundInstance.
  */
  /**************************************************************************/
  void SoundInstance::Resume()
  {
    //Daisy->getSystem<Systems::Audio>()->ResumeSound(*this);

    if (Type == SoundCue::SoundCueType::Event && SoundHandle.EventInstance)
      SoundHandle.EventInstance->setPaused(false);
    else if (Type == SoundCue::SoundCueType::File && SoundHandle.Channel)
      SoundHandle.Channel->setPaused(false);
  }

  /**************************************************************************/
  /*!
  @brief A function that pauses the SoundInstance.
  */
  /**************************************************************************/
  void SoundInstance::Pause()
  {
    if (Type == SoundCue::SoundCueType::Event && SoundHandle.EventInstance)
      SoundHandle.EventInstance->setPaused(true);
    else if (Type == SoundCue::SoundCueType::File && SoundHandle.Channel)
      SoundHandle.Channel->setPaused(true);
  }

  /**************************************************************************/
  /*!
  @brief A function that stops the SoundInstance.
  */
  /**************************************************************************/
  void SoundInstance::Stop()
  {
    if (Type == SoundCue::SoundCueType::Event && SoundHandle.EventInstance)
      SoundHandle.EventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
    else if (Type == SoundCue::SoundCueType::File && SoundHandle.Channel) 
      SoundHandle.Channel->stop();
  }

  /**************************************************************************/
  /*!
  @brief  A function that returns the state of when its playing.
  @return Returns a boolean that results in true if the SoundInstance is 
          currently playing and false if is not.
  */
  /**************************************************************************/
  bool SoundInstance::IsPlaying()
  {
    return !Paused;
  }

  SoundInstance * SoundInstance::Dereference(const Zilch::Handle& handle)
  {
    return reinterpret_cast<SoundInstance*>(handle.Dereference());
  }

  /**************************************************************************/
  /*!
  @brief  Sets the specified parameter on the event instance.
  @param  parameter The event parameter.
  */
  /**************************************************************************/
  void SoundInstance::SetParameter(String parameter, float value)
  {
    if (SoundHandle.EventInstance)
      SoundHandle.EventInstance->setParameterValue(parameter.c_str(), value);
  }

}