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

  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(MusicManager, "MusicManager", Rebound, builder, type) {
    DCE_BINDING_INTERNAL_COMPONENT_SET_ATTRIBUTE_RESOURCE_TYPE(SoundCue);    
    //DCE_BINDING_INTERNAL_COMPONENT_SET_ATTRIBUTE_RESOURCE;    
    DCE_BINDING_DEFINE_PROPERTY(MusicManager, CurrentTrack)->Attributes.push_back(resourceAttribute);
    DCE_BINDING_DEFINE_PROPERTY(MusicManager, LastTrack);
    DCE_BINDING_DEFINE_PROPERTY(MusicManager, FadeInTime);
    DCE_BINDING_DEFINE_PROPERTY(MusicManager, FadeOutTime);
  }
  #endif

  MusicManager::~MusicManager()
  {
    this->SpaceRef->getComponent<SoundSpace>()->StopCue(CurrentTrack);
  }
  void MusicManager::Initialize()
  {
    Connect(Daisy->getKeyboard(), Events::KeyDown, MusicManager::OnKeyDownEvent);
    Connect(Daisy->getKeyboard(), Events::KeyUp, MusicManager::OnKeyUpEvent);
    Connect(Owner() , Events::PlayMusic, MusicManager::OnPlayMusicEvent);
    Daisy->Connect<Events::EnginePause>(&MusicManager::OnEnginePauseEvent, this);
    Daisy->Connect<Events::EngineResume>(&MusicManager::OnEngineResumeEvent, this);
  }

  void MusicManager::OnKeyDownEvent(Events::KeyDown * event)
  {
    switch (event->Key) {

    case Keys::M:
      Daisy->getSystem<Systems::Content>()->getSoundCue(CurrentTrack)->Loop = true;
      this->SpaceRef->getComponent<SoundSpace>()->PlayCue(CurrentTrack);
      break;
    }
  }

  void MusicManager::OnKeyUpEvent(Events::KeyUp * event)
  {
  }

  void MusicManager::OnEnginePauseEvent(Events::EnginePause * event)
  {
    //this->SpaceRef->getComponent<SoundSpace>()->PauseCue(CurrentTrack);
  }

  void MusicManager::OnEngineResumeEvent(Events::EngineResume * event)
  {
    //this->SpaceRef->getComponent<SoundSpace>()->ResumeCue(CurrentTrack);
  }

  void MusicManager::OnPlayMusicEvent(Events::PlayMusic* event)
  {
    Daisy->getSystem<Systems::Content>()->getSoundCue(CurrentTrack)->Loop = true;
    this->SpaceRef->getComponent<SoundSpace>()->PlayCue(CurrentTrack);
  }

}