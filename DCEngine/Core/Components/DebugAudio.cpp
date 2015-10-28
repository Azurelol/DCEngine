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
    auto SoundSpaceRef = SpaceRef->getComponent<SoundSpace>();
       
    switch (event->Key) {
           
    case Keys::F1:
      SoundSpaceRef->StopCue(SoundCue2);
      SoundSpaceRef->PlayCue(SoundCue1);
      break;
    case Keys::F2:
      SoundSpaceRef->StopCue(SoundCue1);
      SoundSpaceRef->PlayCue(SoundCue2);
      break;
    case Keys::F3:
      break;
    case Keys::F4:
      break;
    }


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