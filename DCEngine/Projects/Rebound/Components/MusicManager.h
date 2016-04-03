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
      DCE_DEFINE_PROPERTY(String,   Track);
      DCE_DEFINE_PROPERTY(Real,     Volume);
      DCE_DEFINE_PROPERTY(Boolean,  PlayOnStart);
      DCE_DEFINE_PROPERTY(Boolean,  StopOnEnd);      

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
      void OnLogicUpdateEvent(Events::LogicUpdate * event);

      DCE_BINDING_DECLARE_DERIVED_TYPE(MusicManager, Component);

      //#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      //ZilchDeclareDerivedType(MusicManager, Component);
      //#endif

    private:
      String Track = "NO TRACK SET";
      Real Volume = 1.0;
      Boolean PlayOnStart = true;
      Boolean StopOnEnd = false;
      int PlayingObj = 0;

      
      SoundInstancePtr trackHandle;
    };

  }
}