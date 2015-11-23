/******************************************************************************/
/*!
@file  DebugAudio.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/


#include "DebugAudio.h"
#include "EngineReference.h"

namespace DCEngine {

  void DebugAudio::Initialize()
  {
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
      break;
    case Keys::F4:
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

    auto SoundSpaceRef = SpaceRef->getComponent<SoundSpace>();
    SoundSpaceRef->StopCue(CurrentSoundCue);
    CurrentSoundCue = track;
    SoundSpaceRef->PlayCue(CurrentSoundCue);
  }

  void DebugAudio::OnKeyUpEvent(Events::KeyUp * event)
  {
  }

  void DebugAudio::Serialize(Json::Value & root)
  {
  }

  void DebugAudio::Deserialize(Json::Value & root)
  {
  }


}