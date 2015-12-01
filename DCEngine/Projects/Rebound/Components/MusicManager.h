/*****************************************************************************/
/*!
@file   Musicmanager.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/26/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"

namespace DCEngine {

  class MusicManager : public Component {
  public:
    // Properties
    DCE_DEFINE_PROPERTY(String, CurrentTrack);
    DCE_DEFINE_PROPERTY(String, LastTrack);
    DCE_DEFINE_PROPERTY(Real, FadeInTime);
    DCE_DEFINE_PROPERTY(Real, FadeOutTime);
    // Methods
    MusicManager(Entity& owner) : Component(std::string("MusicManager"), owner) {}
    void Initialize();
    void OnKeyDownEvent(Events::KeyDown* event);
    void OnKeyUpEvent(Events::KeyDown* event);

    DCE_BINDING_DECLARE_DERIVED_TYPE(MusicManager, Component);

    //#if (DCE_USE_ZILCH_INTERNAL_BINDING)
    //ZilchDeclareDerivedType(MusicManager, Component);
    //#endif

  private:
    String CurrentTrack = "NO TRACK SET";
    String LastTrack = "NO TRACK SET";
    Real FadeInTime = 1;
    Real FadeOutTime = 1;
    


  };

}