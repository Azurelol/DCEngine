#include "CameraController.h"
#include "../../CoreComponents.h"

namespace DCEngine {

	void CameraController::Initialize()
	{
		auto gameObj = dynamic_cast<GameObject*>(Owner());
		Connect(Daisy->getMouse(), Events::MouseDown, CameraController::OnMouseDownEvent);
		Connect(Daisy->getMouse(), Events::MouseUp, CameraController::OnMouseUpEvent);
		Connect(SpaceRef, Events::LogicUpdate, CameraController::OnLogicUpdateEvent);
		TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Transform>();
		SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Sprite>();
		PlayerRef = SpaceRef->FindObjectByName("Mariah");
	}

	void CameraController::Serialize(Json::Value & root)
	{
	}

	void CameraController::Deserialize(Json::Value & root)
	{
	}

	void CameraController::OnMouseDownEvent(Events::MouseDown * event)
	{

	}

	void CameraController::OnMouseUpEvent(Events::MouseUp * event)
	{

	}

	void CameraController::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{
		Real FrameRateCompensation = event->Dt * 60;
		//DCTrace << "FRC =" << FrameRateCompensation << "\n";
		auto TargetPos = PlayerRef->getComponent<Transform>()->Translation - TransformRef->Translation;
		TargetPos.z = 0; // do not want the camera moving on the Z axis
		TargetPos.y += 5;
		TransformRef->Translation += InterpolationSpeed * TargetPos * FrameRateCompensation;
	}



	void CameraController::PrintTranslation()
	{
		//DCTrace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x
		//	<< ", " << TransformRef->Translation.y
		//	<< ", " << TransformRef->Translation.z << ")\n";
	}
}