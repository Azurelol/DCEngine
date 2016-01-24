/******************************************************************************/
/*!
@file   FMODSystemPtr.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/9/2015
@brief  
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include <FMOD\fmod_studio.hpp>
#include <FMOD\fmod.hpp>
#include <FMOD\fmod_errors.h>

class FMODSystemPtr {
public:
  FMOD::Studio::System* Studio;
  FMOD::System* LowLevel;
  FMODSystemPtr() : Studio(nullptr)
  {
    // Create the low level API system
    if (FMOD::System_Create(&LowLevel) != FMOD_OK) {
      LowLevel = nullptr;
      //throw something;
    }
    // Create the Studio API system
    //if (FMOD::Studio::System::create(&Handle) != FMOD_OK)
    //  Handle = nullptr;

  }
  FMODSystemPtr(FMODSystemPtr&&) = default;
  FMODSystemPtr& operator=(FMODSystemPtr&&) = default;
  FMODSystemPtr(const FMODSystemPtr&) = delete;
  FMODSystemPtr& operator=(const FMODSystemPtr&) = delete;
  FMOD::Studio::System* operator->() {
    return Studio;
  }
  ~FMODSystemPtr() {
    auto result = LowLevel->release();
    Studio->release();
  }
}; 