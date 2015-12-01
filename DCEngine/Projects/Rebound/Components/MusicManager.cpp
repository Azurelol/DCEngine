/*****************************************************************************/
/*!
@file   MusicManager.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/01/2015
@note   This file includes all the headers for the components that Rebound
uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "MusicManager.h"
#include "ReboundEngineReference.h"

namespace DCEngine {

  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(MusicManager, "MusicManager", Rebound, builder, type) {
    DCE_BINDING_DEFINE_PROPERTY(MusicManager, CurrentTrack);
    DCE_BINDING_DEFINE_PROPERTY(MusicManager, LastTrack);
    DCE_BINDING_DEFINE_PROPERTY(MusicManager, FadeInTime);
    DCE_BINDING_DEFINE_PROPERTY(MusicManager, FadeOutTime);
  }
  #endif

  void MusicManager::Initialize()
  {
  }

  void MusicManager::OnKeyDownEvent(Events::KeyDown * event)
  {
  }

  void MusicManager::OnKeyUpEvent(Events::KeyDown * event)
  {
  }

}