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
      DCE_BINDING_DEFINE_ATTRIBUTE(SoundCue);
      DCE_BINDING_DEFINE_PROPERTY(MusicManager, CurrentTrack); // ->Attributes.push_back(attributeSoundCue);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyCurrentTrack, attributeSoundCue);
      DCE_BINDING_DEFINE_PROPERTY(MusicManager, LastTrack); // ->Attributes.push_back(attributeSoundCue);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyLastTrack, attributeSoundCue);
      DCE_BINDING_DEFINE_PROPERTY(MusicManager, FadeInTime);
      DCE_BINDING_DEFINE_PROPERTY(MusicManager, FadeOutTime);
    }
#endif

    MusicManager::~MusicManager()
    {
      if (play >= 3)
        this->SpaceRef->getComponent<Components::SoundSpace>()->StopCue(CurrentTrack);
    }
    void MusicManager::Initialize()
    {
      Connect(Daisy->getKeyboard(), Events::KeyDown, MusicManager::OnKeyDownEvent);
      Connect(Daisy->getKeyboard(), Events::KeyUp, MusicManager::OnKeyUpEvent);
      Connect(Owner(), Events::PlayMusic, MusicManager::OnPlayMusicEvent);
      Connect(SpaceRef, Events::LogicUpdate, MusicManager::OnLogicUpdateEvent);
      play = true;
    }

    void MusicManager::OnKeyDownEvent(Events::KeyDown * event)
    {
      switch (event->Key) {

      case Keys::M:
        Daisy->getSystem<Systems::Content>()->getSoundCue(CurrentTrack)->Loop = true;
        this->SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(CurrentTrack);
        break;
      }
    }

    void MusicManager::OnKeyUpEvent(Events::KeyUp * event)
    {
    }

    void MusicManager::OnPlayMusicEvent(Events::PlayMusic* event)
    {
      //Daisy->getSystem<Systems::Content>()->getSoundCue(CurrentTrack)->Loop = true;
      //this->SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(CurrentTrack);
    }

    void MusicManager::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
      if (Daisy->getSystem<Systems::Content>()->getSoundCue(CurrentTrack) && play < 3)
      {
        Daisy->getSystem<Systems::Content>()->getSoundCue(CurrentTrack)->Loop = true;
        this->SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(CurrentTrack);
      }

      ++play;
    }
  }
}