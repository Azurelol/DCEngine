/******************************************************************************/
/*!
\file   FMODPtrs.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/16/2015
\brief  Encapsulates a raw FMOD pointer to behave like a smart pointer,
        releasing its resource when destroyed.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include <FMOD\fmod.hpp>
#include <FMOD\fmod_errors.h>
#include <FMOD/fmod_studio.hpp>

namespace DCEngine {
  
  class FMODSoundHandle {
  
  public:

    FMOD::Channel* Channel;
    FMOD::Sound* Handle;
    FMOD::Studio::EventInstance* EventInstance;

    FMODSoundHandle() : Channel(nullptr), Handle(nullptr), EventInstance(nullptr) {}
    FMOD::Sound* operator->() { return Handle; }
    // What do these do, again? 
    //FMODSoundHandle(FMODSoundHandle&&) = default;
    //FMODSoundHandle& operator=(FMODSoundHandle&&) = default;
    
    //FMODSoundHandle(const FMODSoundHandle&) = delete;
    //FMODSoundHandle& operator=(const FMODSoundHandle&) = delete;

    ~FMODSoundHandle() {

      // WHY DOTH YOU CRASH
      //auto result = Handle->release();
    }
  };

}