/*****************************************************************************/
/*!
@file   SoundCue.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/04/2015
@brief  The SoundCue resource encapsulates a sound file.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"
// Headers
#include "../Systems/Audio/FMODPtrs.h"

namespace DCEngine {

  class SoundCue;
  using SoundCuePtr = std::shared_ptr<SoundCue>;

  class SoundCue : public Resource {

  public:

    bool Loop = false;
    FMODSoundPtr Data;

    SoundCue(std::string soundFile);
    void Load();
    static SoundCuePtr Find(std::string);

  private:
    const std::string SoundFileName;

    //FMODSoundPtr SoundPtr; //!< A hand-written smart pointer for FMOD::Sound*.
  };


 
}