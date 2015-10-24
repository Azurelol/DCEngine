#include "DebugMoveController.h"
#include "EngineReference.h"
#include "Transform.h"

// Access to the SoundSpace to play sounds
#include "SoundSpace.h"

namespace DCEngine {
  
  void DebugMoveController::Initialize()
  {
    Connect(Daisy->getKeyboard(), Events::KeyDown, DebugMoveController::OnKeyDownEvent);
    Connect(Daisy->getKeyboard(), Events::KeyUp, DebugMoveController::OnKeyUpEvent);
    Connect(Daisy->getMouse(), Events::MouseDown, DebugMoveController::OnMouseDownEvent);
    Connect(SpaceRef, Events::LogicUpdate, DebugMoveController::OnLogicUpdateEvent);

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
      TransformRef->Translation.y += MoveSpeed;
      PrintTranslation();
      break;    
    case Keys::S:
      TransformRef->Translation.y -= MoveSpeed;
      PrintTranslation();
      break;
    case Keys::A:
      TransformRef->Translation.x -= MoveSpeed;
      PrintTranslation();
      break;
    case Keys::D:
      TransformRef->Translation.x += MoveSpeed;
      PrintTranslation();
      break;
    case Keys::E:
      TransformRef->Translation.z -= MoveSpeed;
      PrintTranslation();
      break;
    case Keys::Q:
      TransformRef->Translation.z += MoveSpeed;
      PrintTranslation();
      break;
    case Keys::Z:
      TransformRef->Rotation.z -= RotSpeed;
      break;
    case Keys::X:
      TransformRef->Rotation.z += RotSpeed;
    }

    if (FootstepSoundEnabled)
      PlayFootstepSound();
    
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

  void DebugMoveController::OnLogicUpdateEvent(Events::LogicUpdate * event)
  {
    //if (Daisy->getKeyboard()->KeyIsDown(Keys::Space))
    //  trace << "DebugMoveController::OnLogicUpdateEvent - SPACE IS DOWN!!!\n";
    //else
    //  trace << "DebugMoveController::OnLogicUpdateEvent - SPACE IS UP!!!\nwa";
    //if (Daisy->getMouse()->MouseDown(MouseButton::Left))
    //  trace << "DebugMoveController::OnLogicUpdateEvent - MOUSELEFT IS DOWN!!!\n";
    //else
    //  trace << "DebugMoveController::OnLogicUpdateEvent - MOUSELEFT IS UP!!!\nwa";

    //if (Daisy->getMouse()->MouseDown(MouseButton::Right))
    //  trace << "DebugMoveController::OnLogicUpdateEvent - MOUSERIGHT IS DOWN!!!\n";
    //else
    //  trace << "DebugMoveController::OnLogicUpdateEvent - MOUSERIGHT IS UP!!!\nwa";
  }

  void DebugMoveController::SetFootstepSound(std::string & soundfileName) {

    // Save the footstep sound
    FootstepSound = soundfileName;
    // Now that a footstep sound has been set, this component will play footsteps
    FootstepSoundEnabled = true;
  }

  void DebugMoveController::PlayFootstepSound()
  {
    //SpaceRef->getComponent<SoundSpace>()->
  }

  void DebugMoveController::PrintTranslation()
  {
    //trace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x
    //  << ", " << TransformRef->Translation.y
    //  << ", " << TransformRef->Translation.z << ")\n";
  }

}