/******************************************************************************/
/*!
\file   DebugAudio.cpp
\author Chen Shu, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/20/2015
\brief  Manages a viewport, attacheing a camera.
\note   This component is accessed by the graphics system for its
projection matrix.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "DebugAudio.h"
#include "EngineReference.h"

namespace DCEngine {
  namespace Components
  {

    ZilchDefineType(DebugAudio, "DebugAudio", DCEngineCore, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(DebugAudio);
      DCE_BINDING_DEFINE_PROPERTY(DebugAudio, Track1);
      DCE_BINDING_DEFINE_PROPERTY(DebugAudio, Track2);
      DCE_BINDING_DEFINE_PROPERTY(DebugAudio, Track3);
    }

    DebugAudio::~DebugAudio()
    {
      //if (this->MyJam)
      //  this->MyJam->Stop();
    }

    void DebugAudio::Initialize()
    {
      Daisy->Connect<Events::KeyDown>(SpaceRef, &DebugAudio::OnKeyDownEvent, this);

      Connect(Daisy->getKeyboard(), Events::KeyDown, DebugAudio::OnKeyDownEvent);
      Connect(Daisy->getKeyboard(), Events::KeyUp, DebugAudio::OnKeyUpEvent);
    }

    void DebugAudio::OnKeyDownEvent(Events::KeyDown * event)
    {
      switch (event->Key) {

      case Keys::F1:
        ChangeTrack(Track1);
        break;
      case Keys::F2:
        ChangeTrack(Track2);
        break;
      case Keys::F3:
        Pause(true);
        break;
      case Keys::F4:
        Pause(false);
        break;
      case Keys::Add:
        ChangeVolume(0.1f);
        break;
      case Keys::Subtract:
        ChangeVolume(-0.1f);
        break;
        
      }
    }

    /**************************************************************************/
    /*!
    @brief  Changes the currently playing track.
    @note   The name of the track (sound file) to play.
    */
    /**************************************************************************/
    void DebugAudio::ChangeTrack(std::string & track)
    {
      if (CurrentSoundCue == track)
        return;

      if (this->MyJam)
        this->MyJam->Stop();
      this->MyJam = SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(track);

      Bank::Find("Master")->VCA("VCAlol")->setFaderLevel(0.0f);
      Bank::Find("Master")->Bus("lol")->setMute(true);
      //auto SoundSpaceRef = SpaceRef->getComponent<Components::SoundSpace>();
      //SoundSpaceRef->StopCue(CurrentSoundCue);
      //CurrentSoundCue = track;
      //SoundSpaceRef->PlayCue(CurrentSoundCue);
    }

    void DebugAudio::ChangeVolume(float increment)
    {
      if (!this->MyJam)
        return;

      this->MyJam->InterpolateVolume(this->MyJam->Settings.Volume + increment, 0);
    }

    void DebugAudio::Pause(bool paused)
    {
      if (!this->MyJam)
        return;
      
      if (paused)
        this->MyJam->Pause();
      else
        this->MyJam->Resume();
    }

    void DebugAudio::OnKeyUpEvent(Events::KeyUp * event)
    {
    }

  }

}