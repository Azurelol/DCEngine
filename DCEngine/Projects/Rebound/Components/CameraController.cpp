/******************************************************************************/
/*!
@file  CameraController .cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/23/2015
@brief Controller for the camera object, handles following the player.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "CameraController.h"
#include "../../CoreComponents.h"

namespace DCEngine {
  namespace Components {
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(CameraController, "CameraController", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(CameraController);
      DCE_BINDING_DEFINE_PROPERTY(CameraController, TargetName);
      DCE_BINDING_DEFINE_PROPERTY(CameraController, InterpolationSpeed);
    }
#endif

    void CameraController::Initialize()
    {
      auto gameObj = dynamic_cast<GameObject*>(Owner());
      Connect(Daisy->getKeyboard(), Events::KeyDown, CameraController::OnKeyDownEvent);
      Connect(Daisy->getMouse(), Events::MouseDown, CameraController::OnMouseDownEvent);
      Connect(Daisy->getMouse(), Events::MouseUp, CameraController::OnMouseUpEvent);
      Connect(SpaceRef, Events::LogicUpdate, CameraController::OnLogicUpdateEvent);
      TransformRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>();
      SpriteRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Sprite>();
      PlayerRef = SpaceRef->FindObjectByName(TargetName);
      //PlayerRef = SpaceRef->FindObjectByName(TargetName);
      //gameObj->AttachTo(PlayerRef);
    //hardcoded and bad, fix
    }
    
    void CameraController::OnKeyDownEvent(Events::KeyDown * event)
    {
      if (event->Key == Keys::A)
      {
        PlayerRef = SpaceRef->FindObjectByName(TargetName);
        //DCTrace << "A key pressed";
      }

    }

    void CameraController::OnMouseDownEvent(Events::MouseDown * event)
    {

    }

    void CameraController::OnMouseUpEvent(Events::MouseUp * event)
    {

    }

    void CameraController::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
      if (DoScreenShake)
      {
        TransformRef->setTranslation(TransformRef->getTranslation() + Vec3((rand() % 100 - 50) / 10.0f, (rand() % 100 - 50) / 10.0f, 0));
      }
      Real FrameRateCompensation = event->Dt * 60;
      if (CameraControllerTraceOn)
      {
        DCTrace << "CameraController::OnLogicUpdateEvent - FRC =" << FrameRateCompensation << "\n";
      }
      if (!PlayerRef)
      {
        PlayerRef = SpaceRef->FindObjectByName(TargetName);
        return;
      }

      //if (PlayerRef->HasComponent<Components::PlayerController>() )
      //  DoScreenShake = PlayerRef->getComponent<Components::PlayerController>()->Dead;

      auto TargetPos = PlayerRef->getComponent<Components::Transform>()->Translation - TransformRef->Translation;
      if (CameraControllerTraceOn)
      {
        PrintTranslation();

        DCTrace << Owner()->Name() << "::Transform.Translation(" << PlayerRef->getComponent<Components::Transform>()->Translation.x
          << ", " << PlayerRef->getComponent<Components::Transform>()->Translation.y
          << ", " << PlayerRef->getComponent<Components::Transform>()->Translation.z << ")\n";
      }
      TargetPos.z = 0; // do not want the camera moving on the Z axis
      TargetPos.y += 5;
      TransformRef->Translation += InterpolationSpeed * TargetPos * FrameRateCompensation;
    }



    void CameraController::PrintTranslation()
    {
      DCTrace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x
        << ", " << TransformRef->Translation.y
        << ", " << TransformRef->Translation.z << ")\n";
    }
  }

}