#include "DebugReport.h"
#include "EngineReference.h"
#include "Transform.h"

namespace DCEngine {
    
  void DebugReport::Initialize() {   
    Connect(Daisy->getKeyboard(), Events::KeyDown, DebugReport::OnKeyDownEvent);    
    TransformRef = dynamic_cast<GameObject*>(owner_)->getComponent<Transform>();
  }

  void DebugReport::Serialize(Json::Value & root)
  {
  }

  void DebugReport::Deserialize(Json::Value & root)
  {
  }

  void DebugReport::OnKeyDownEvent(Events::KeyDown * event)
  {
    // Keyboard Input broken?
    //switch (event->Key) {
    //case Keys::W:
      trace << Owner()->Name() << " is now at (" << TransformRef->Translation.x << ", "
                                                 << TransformRef->Translation.y << ", "
                                                 << TransformRef->Translation.z << ")\n";
    //  break;    
    //}
  }

  void DebugReport::OnKeyUpEvent(Events::KeyUp * event)
  {
  }

  void DebugReport::OnMouseDownEvent(Events::MouseDown * event)
  {
  }

}