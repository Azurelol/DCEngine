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

  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(CameraController, "CameraController", Rebound, builder, type) {
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
		TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Transform>();
		SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Sprite>();
    PlayerRef = SpaceRef->FindObjectByName("Player");
    //PlayerRef = SpaceRef->FindObjectByName(TargetName);
    //gameObj->AttachTo(PlayerRef);
	}

	void CameraController::Serialize(Json::Value & root)
	{
	}

	void CameraController::Deserialize(Json::Value & root)
	{
	}

	void CameraController::OnKeyDownEvent(Events::KeyDown * event)
	{
		if (event->Key == Keys::A)
		{
			PlayerRef = SpaceRef->FindObjectByName("Player");
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
    return;
		Real FrameRateCompensation = event->Dt * 60;
		if (CameraControllerTraceOn)
		{
			DCTrace << "FRC =" << FrameRateCompensation << "\n";
		}
		auto TargetPos = PlayerRef->getComponent<Transform>()->Translation - TransformRef->Translation;
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