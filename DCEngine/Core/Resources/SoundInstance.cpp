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

  /**************************************************************************/
  /*!
  @brief  SoundInstance constructor.
  */
  /**************************************************************************/
  SoundInstance::SoundInstance() : SoundInstanceID(Created++)
  {
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
    Daisy->getSystem<Systems::Audio>()->ResumeSound(*this);
  }

  /**************************************************************************/
  /*!
  @brief A function that pauses the SoundInstance.
  */
  /**************************************************************************/
  void SoundInstance::Pause()
  {
    
  }

  /**************************************************************************/
  /*!
  @brief A function that stops the SoundInstance.
  */
  /**************************************************************************/
  void SoundInstance::Stop()
  {
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

}