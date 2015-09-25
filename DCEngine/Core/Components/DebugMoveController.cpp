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
      TransformRef->Translation.y += MoveSpeed;
      break;    
    case Keys::S:
      trace << _name << "::OnKeyDownEvent - Pressed: 'S' \n";
      TransformRef->Translation.y -= MoveSpeed;
      break;
    case Keys::A:
      trace << _name << "::OnKeyDownEvent - Pressed: 'A' \n";
      TransformRef->Translation.x -= MoveSpeed;
      break;
    case Keys::D:
      trace << _name << "::OnKeyDownEvent - Pressed: 'D' \n";
      TransformRef->Translation.x += MoveSpeed;
      break;
    case Keys::E:
      trace << _name << "::OnKeyDownEvent - Pressed: 'Q' \n";
      TransformRef->Translation.z -= MoveSpeed;
      break;
    case Keys::Q:
      trace << _name << "::OnKeyDownEvent - Pressed: 'E' \n";
      TransformRef->Translation.z += MoveSpeed;
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

  void DebugMoveController::SetFootstepSound(std::string & soundfileName) {

    // Save the footstep sound
    FootstepSound = soundfileName;
    // Now that a footstep sound has been set, this component will play footsteps
    FootstepSoundEnabled = true;
  }

  void DebugMoveController::PlayFootstepSound()
  {
    //space_->getComponent<SoundSpace>()->
  }

}