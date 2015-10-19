#include "PlayerController.h"
#include "../../CoreComponents.h"

namespace DCEngine {

	void PlayerController::Initialize()
	{
		Connect(Daisy->getMouse(), Events::MouseDown, PlayerController::OnMouseDownEvent);
		Connect(Daisy->getMouse(), Events::MouseUp, PlayerController::OnMouseUpEvent);
		Connect(SpaceRef, Events::LogicUpdate, PlayerController::OnLogicUpdateEvent);
		TransformRef = dynamic_cast<GameObject*>(owner_)->getComponent<Transform>(); // ew
		RigidBodyRef = dynamic_cast<GameObject*>(owner_)->getComponent<RigidBody>();
	}

	void PlayerController::Serialize(Json::Value & root)
	{
	}

	void PlayerController::Deserialize(Json::Value & root)
	{
	}

	void PlayerController::OnMouseDownEvent(Events::MouseDown * event)
	{
		Charging = true;
	}
	void PlayerController::OnMouseUpEvent(Events::MouseUp * event)
	{
		Real3 MouseVector = Real3(event->Position.x, event->Position.y, 0); //set to the actual mouse's normalized vector once we have that capability
		RigidBodyRef->AddForce(MouseVector * ChargeFactor * CurrentCharge);
		Charging = false;
		CurrentCharge = 0;
		trace << "Mouse up! \n";
	}

	void PlayerController::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{
		if (Charging)
		{
			CurrentCharge += event->Dt;
			trace << CurrentCharge << "\n";
			if (CurrentCharge > MaxCharge)
			{
				CurrentCharge = MaxCharge;
			}
		}
		PrintVelocity();

	}



	void PlayerController::PrintTranslation()
	{
		//trace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x	<< ", " << TransformRef->Translation.y << ", " << TransformRef->Translation.z << ")\n";
	}

	void PlayerController::PrintVelocity()
	{
		Real3 vel = RigidBodyRef->getVelocity();
		//trace << Owner()->Name() << "::RigidBody.Velocity(" << vel.x << ", " << vel.y<< ", " << vel.z << ")\n";
	}
}