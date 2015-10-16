#include "BallController.h"
#include "EngineReference.h"
#include "Transform.h"
#include "RigidBody.h"

// Access to the SoundSpace to play sounds
#include "SoundSpace.h"

namespace DCEngine {

	void BallController::Initialize()
	{
		Connect(Daisy->getMouse(), Events::MouseDown, BallController::OnMouseDownEvent);
		Connect(Daisy->getMouse(), Events::MouseUp, BallController::OnMouseUpEvent);
		Connect(SpaceRef, Events::LogicUpdate, BallController::OnLogicUpdateEvent);
		TransformRef = dynamic_cast<GameObject*>(owner_)->getComponent<Transform>(); // ew
		RigidBodyRef = dynamic_cast<GameObject*>(owner_)->getComponent<RigidBody>();
	}

	void BallController::Serialize(Json::Value & root)
	{
	}

	void BallController::Deserialize(Json::Value & root)
	{
	}

	void BallController::OnMouseDownEvent(Events::MouseDown * event) 
	{
		Charging = true;
	}
	void BallController::OnMouseUpEvent(Events::MouseUp * event)
	{
		Real3 MouseVector = Real3(event->x, event->y, 0); //set to the actual mouse's normalized vector once we have that capability
		RigidBodyRef->AddForce(MouseVector * ChargeFactor * CurrentCharge);
		Charging = false;
		CurrentCharge = 0;
		trace << "Mouse up! \n";
	}

	void BallController::OnLogicUpdateEvent(Events::LogicUpdate * event)
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



	void BallController::PrintTranslation()
	{
		trace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x
			<< ", " << TransformRef->Translation.y
			<< ", " << TransformRef->Translation.z << ")\n";
	}

	void BallController::PrintVelocity()
	{
		Real3 vel = RigidBodyRef->getVelocity();
		trace << Owner()->Name() << "::RigidBody.Velocity(" << vel.x << ", " << vel.y<< ", " << vel.z << ")\n";
	}
}