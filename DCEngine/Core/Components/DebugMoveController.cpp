#include "DebugMoveController.h"
#include "EngineReference.h"
#include "Transform.h"

namespace DCEngine {
  
  void DebugMoveController::Initialize()
  {
    Connect(Daisy->getKeyboard(), Events::KeyDown, DebugMoveController::OnKeyDownEvent);
    Connect(Daisy->getKeyboard(), Events::KeyUp, DebugMoveController::OnKeyUpEvent);
    Connect(Daisy->getMouse(), Events::MouseDown, DebugMoveController::OnMouseDownEvent);

    TransformRef = dynamic_cast<GameObject*>(owner_)->getComponent<Transform>(); // ew
    

  }

  void DebugMoveController::Serialize(Json::Value & root)
  {
  }

  void DebugMoveController::Deserialize(Json::Value & root)
  {
  }

  void DebugMoveController::OnKeyDownEvent(Events::KeyDown* event) {
    switch (event->Key) {
    case Keys::W:
      trace << _name << "::OnKeyDownEvent - Pressed: 'W' \n";
      TransformRef->Translation.z -= MoveSpeed;
      break;    
    case Keys::S:
      trace << _name << "::OnKeyDownEvent - Pressed: 'S' \n";
      TransformRef->Translation.z += MoveSpeed;
      break;
    case Keys::A:
      trace << _name << "::OnKeyDownEvent - Pressed: 'A' \n";
      TransformRef->Translation.x -= MoveSpeed;
      break;
    case Keys::D:
      trace << _name << "::OnKeyDownEvent - Pressed: 'D' \n";
      TransformRef->Translation.x += MoveSpeed;
      break;
    }

    trace << Owner()->Name() << " is now at (" << TransformRef->Translation.x << ", " 
                                               << TransformRef->Translation.y << ", "
                                                << TransformRef->Translation.z << ")\n";

  }

  void DebugMoveController::OnKeyUpEvent(Events::KeyUp* event) {
    
    if (event->Key == Keys::Up) {
      trace << "'Up' key was released!\n";
    }
    else if (event->Key == Keys::Down) {
      trace << "'Down' key was released!\n";
    }
    else if (event->Key == Keys::Left) {
      trace << "'Left' key was released!\n";
    }
    else if (event->Key == Keys::Right) {
      trace << "'Right' key was released!\n";
    }
  }

  void DebugMoveController::OnMouseDownEvent(Events::MouseDown * event) {
    trace << "Mouse down!\n";

  }

}