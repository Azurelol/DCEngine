#include "DebugCamera.h"
#include "EngineReference.h"
#include "Camera.h"
#include "Transform.h"


namespace DCEngine {
   
  void DebugCamera::Initialize()
  {
    CameraComponent = dynamic_cast<GameObject*>(owner_)->getComponent<Camera>();
    TransformComponent = dynamic_cast<GameObject*>(owner_)->getComponent<Transform>();
    Connect(Daisy->getKeyboard(), Events::KeyDown, DebugCamera::OnKeyDownEvent);
    Connect(Daisy->getKeyboard(), Events::KeyUp, DebugCamera::OnKeyUpEvent);
  }

  void DebugCamera::OnKeyDownEvent(Events::KeyDown * event)
  {
    switch (event->Key) {

    /* Zoom */
    case Keys::Add:
      CameraComponent->Size += ZoomSpeed;
      trace << Owner()->Name() << " - Size: " << CameraComponent->Size << "\n";
      break;
    case Keys::Subtract:
      CameraComponent->Size -= ZoomSpeed;
      trace << Owner()->Name() << " - Size: " << CameraComponent->Size << "\n";
      break;

      /* Movement */
    case Keys::Up:
      TransformComponent->Translation.y += MoveSpeed;
      break;
    case Keys::Down:
      TransformComponent->Translation.y -= MoveSpeed;
      break;
    case Keys::Left:
      TransformComponent->Translation.x -= MoveSpeed;
      break;
    case Keys::Right:
      TransformComponent->Translation.x += MoveSpeed;
      break;
    }
  }

  void DebugCamera::OnKeyUpEvent(Events::KeyUp * event)
  {
  }

  void DebugCamera::Serialize(Json::Value & root)
  {
  }

  void DebugCamera::Deserialize(Json::Value & root)
  {
  }


}