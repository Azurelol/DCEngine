/*****************************************************************************/
/*!
@file   SoundCue.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/04/2015
@brief  The SoundCue resource encapsulates a sound file.
*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"

// Headers
#include "../Systems/Audio/FMODPtrs.h"
//#include <FMOD\fmod.hpp>
//#include <FMOD\fmod_errors.h>

//using FMODSoundPtr = std::unique_ptr<FMOD::Sound*>;

namespace DCEngine {

  class SoundCue : public Resource {

  public:

    bool Loop = false;
    FMODSoundPtr SoundPtr;

    SoundCue(std::string soundFile);
    void Load();

  private:
    const std::string SoundFileName;

    //FMODSoundPtr SoundPtr; //!< A hand-written smart pointer for FMOD::Sound*.
  };


  using SoundCuePtr = std::shared_ptr<SoundCue>;
}