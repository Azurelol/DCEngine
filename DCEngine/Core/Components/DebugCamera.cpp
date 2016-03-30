/******************************************************************************/
/*!
@file   DebugCamera.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/12/2015
@brief  Debug Camera component implementation.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "DebugCamera.h"
#include "EngineReference.h"
#include "Camera.h"
#include "Transform.h"


namespace DCEngine {
  namespace Components
  {
    void DebugCamera::Initialize()
    {
      CameraComponent = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Camera>();
      TransformComponent = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>();
      Connect(Daisy->getKeyboard(), Events::KeyDown, DebugCamera::OnKeyDownEvent);
      Connect(Daisy->getKeyboard(), Events::KeyUp, DebugCamera::OnKeyUpEvent);
    }

    void DebugCamera::OnKeyDownEvent(Events::KeyDown * event)
    {
      switch (event->Key) {

        /* Zoom */
      case Keys::Add:
        if (CameraComponent->Projection == ProjectionMode::Perspective)
        {
          CameraComponent->FieldOfView -= 10;
          DCTrace << Owner()->Name() << " - FieldOfView: " << CameraComponent->FieldOfView << "\n";
        }
        else
        {
          CameraComponent->Size += 1;
          DCTrace << Owner()->Name() << " - Size: " << CameraComponent->FieldOfView << "\n";
        }
        break;
      case Keys::Subtract:
        if (CameraComponent->Projection == ProjectionMode::Perspective)
        {
          CameraComponent->FieldOfView += 10;
          DCTrace << Owner()->Name() << " - FieldOfView: " << CameraComponent->FieldOfView << "\n";
        }
        else
        {
          CameraComponent->Size -= 1;
          DCTrace << Owner()->Name() << " - Size: " << CameraComponent->FieldOfView << "\n";
        }
        break;

        /* Movement */
      case Keys::Up:
        TransformComponent->Translation.y += MoveSpeed;
        PrintTranslation();
        break;
      case Keys::Down:
        TransformComponent->Translation.y -= MoveSpeed;
        PrintTranslation();
        break;
      case Keys::Left:
        TransformComponent->Translation.x -= MoveSpeed;
        PrintTranslation();
        break;
      case Keys::Right:
        TransformComponent->Translation.x += MoveSpeed;
        PrintTranslation();
        break;
      case Keys::F1:
        TransformComponent->Rotation.z -= RotSpeed;
        break;
      case Keys::F2:
        TransformComponent->Rotation.z += RotSpeed;

      }


    }

    void DebugCamera::PrintTranslation() {
      DCTrace << Owner()->Name() << "::Transform.Translation(" << TransformComponent->Translation.x
        << ", " << TransformComponent->Translation.y
        << ", " << TransformComponent->Translation.z << ")\n";
    }

    void DebugCamera::OnKeyUpEvent(Events::KeyUp * event)
    {
    }


  }

}