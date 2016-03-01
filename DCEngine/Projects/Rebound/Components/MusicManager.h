/*****************************************************************************/
/*!
@file   MusicManager.h
@author Christian Sagel, Blaine Reiner
@par    email: c.sagel\@digipen.edu , blaine.reiner\@digipen.edu
@date   11/26/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"
#include "../ReboundEvents.h"

namespace DCEngine {
  namespace Components {
    class MusicManager : public Component {
    public:
      // Properties
      DCE_DEFINE_PROPERTY(String, CurrentTrack);
      DCE_DEFINE_PROPERTY(String, LastTrack);
      DCE_DEFINE_PROPERTY(Real, FadeInTime);
      DCE_DEFINE_PROPERTY(Real, FadeOutTime);
      DCE_DEFINE_PROPERTY(Real, Volume);
      DCE_DEFINE_PROPERTY(Boolean, PlayOnStart);

      // Methods
      MusicManager(Entity& owner) : Component(std::string("MusicManager"), owner) {}
      ~MusicManager();
      void Initialize();
      void PlayMusic();
      void PauseMusic();
      void StopMusic();
      void OnGameStartedEvent(Events::GameStarted* event);
      void OnKeyDownEvent(Events::KeyDown* event);
      void OnPlayMusicEvent(Events::PlayMusic* event);
      void OnKeyUpEvent(Events::KeyUp* event);
      void OnLogicUpdateEvent(Events::LogicUpdate * event);

      DCE_BINDING_DECLARE_DERIVED_TYPE(MusicManager, Component);

      //#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      //ZilchDeclareDerivedType(MusicManager, Component);
      //#endif

    private:
      String CurrentTrack = "NO TRACK SET";
      String LastTrack = "NO TRACK SET";
      Real Volume = 1.0;
      Real FadeInTime = 0;
      Real FadeOutTime = 0;
      int play = 0;
      Boolean PlayOnStart = true;

    };

  }
}