/******************************************************************************/
/*!
\file   FMODPtrs.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/16/2015
\brief  Encapsulates a raw FMOD pointer to behave like a smart pointer,
        releasing its resource when destroyed.
*/
/******************************************************************************/
#pragma once
#include <FMOD\fmod.hpp>
#include <FMOD\fmod_errors.h>

namespace DCEngine {
  
  class FMODSoundPtr {
  
  public:

    FMOD::Channel* Channel;
    FMOD::Sound* SoundPtr;

    FMODSoundPtr() {}
    // What do these do, again? 
    //FMODSoundPtr(FMODSoundPtr&&) = default;
    //FMODSoundPtr& operator=(FMODSoundPtr&&) = default;
    
    //FMODSoundPtr(const FMODSoundPtr&) = delete;
    //FMODSoundPtr& operator=(const FMODSoundPtr&) = delete;

    ~FMODSoundPtr() {

      // WHY DOTH YOU CRASH
      //auto result = SoundPtr->release();
    }
  };

}