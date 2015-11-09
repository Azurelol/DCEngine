/******************************************************************************/
/*!
@file   FMODSystemPtr.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/9/2015
@brief  
*/
/******************************************************************************/
#pragma once
#include <FMOD\fmod.hpp>
#include <FMOD\fmod_errors.h>

class FMODSystemPtr {
public:
  FMOD::System* ptr;
  FMODSystemPtr() {
    auto result = FMOD::System_Create(&ptr);
    if (result != FMOD_OK) {
      ptr = nullptr;
      //throw something;
    }
  }
  FMODSystemPtr(FMODSystemPtr&&) = default;
  FMODSystemPtr& operator=(FMODSystemPtr&&) = default;
  FMODSystemPtr(const FMODSystemPtr&) = delete;
  FMODSystemPtr& operator=(const FMODSystemPtr&) = delete;
  ~FMODSystemPtr() {
    auto result = ptr->release();    
  }
}; 