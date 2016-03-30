/*****************************************************************************/
/*!
@file   MusicManager.cpp
@author Christian Sagel, Blaine Reiner
@par    email: c.sagel\@digipen.edu , blaine.reiner\@digipen.edu
@date   11/26/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "MusicManager.h"
#include "ReboundEngineReference.h"

namespace DCEngine {
  namespace Components {
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(MusicManager, "MusicManager", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(MusicManager);
      DCE_BINDING_DEFINE_PROPERTY(MusicManager, Track);
      DCE_BINDING_DEFINE_PROPERTY(MusicManager, Volume);
      DCE_BINDING_DEFINE_PROPERTY(MusicManager, PlayOnStart);
      DCE_BINDING_DEFINE_PROPERTY(MusicManager, StopOnEnd);
    }
#endif

    MusicManager::~MusicManager()
    {
      if (this->getObjectID() == PlayingObj)
      {
        SpaceRef->getComponent<Components::SoundSpace>()->StopCue(Track);
      }
    }

    void MusicManager::Initialize()
    {
      Connect(Owner(), Events::PlayMusic, MusicManager::OnPlayMusicEvent);
      Connect(Owner(), Events::KeyDown, MusicManager::OnKeyDownEvent);
      Connect(SpaceRef, Events::LogicUpdate, MusicManager::OnLogicUpdateEvent);

      if (PlayOnStart == true)
      {
        SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(Track);
        PlayingObj = this->getObjectID();
      }
    }

    void MusicManager::OnKeyDownEvent(Events::KeyDown * event)
    {
      switch (event->Key) {

      case Keys::M:
        SpaceRef->getComponent<Components::SoundSpace>()->PauseCue(Track);
        break;
      }
    }

    void MusicManager::OnPlayMusicEvent(Events::PlayMusic* event)
    {

    }

    void MusicManager::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {

    }
  }
}
