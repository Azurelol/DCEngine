#include "BallController.h"
#include "../../CoreComponents.h"

namespace DCEngine {

	void BallController::Initialize()
	{
		Connect(Daisy->getMouse(), Events::MouseDown, BallController::OnMouseDownEvent);
		Connect(Daisy->getMouse(), Events::MouseUp, BallController::OnMouseUpEvent);
		Connect(SpaceRef, Events::LogicUpdate, BallController::OnLogicUpdateEvent);
		TransformRef = dynamic_cast<GameObject*>(owner_)->getComponent<Transform>(); // ew
		RigidBodyRef = dynamic_cast<GameObject*>(owner_)->getComponent<RigidBody>();
		//SpriteRef = dynamic_cast<GameObject*>(owner_)->getComponent<Sprite>();
		PlayerRef = SpaceRef->FindObjectByName("Mariah");
		trace << "#################################################";
		trace << PlayerRef->getComponent<Transform>()->Translation.x;
	}

	void BallController::Serialize(Json::Value & root)
	{
	}

	void BallController::Deserialize(Json::Value & root)
	{
	}

	void BallController::OnMouseDownEvent(Events::MouseDown * event) 
	{
    trace << "BallController::OnMouseDownEvent - Mouse Pos (Pixels): " 
          << event->Position.x << " y: " << event->Position.y << "\n";

    // Call the CameraViewport component of the space with this screen position.. ?
    auto coords = SpaceRef->getComponent<CameraViewport>()->ScreenToViewport(Vec2(event->Position));
    trace << "BallController::OnMouseDownEvent - Mouse Pos (World): "
      << coords.x << " y: " << coords.y << "\n";

		if (CurrentlyFired)
		{

		}
		else
		{
			Charging = true;
		}
		PlayerRef->getComponent<Sprite>()->Color = Vec4(1, 1, 1, 1);
	}

	void BallController::OnMouseUpEvent(Events::MouseUp * event)
	{ 
		if (!CurrentlyFired)
		{
			Vec3 MouseVector = Vec3(event->Position.x, event->Position.y, 0); //set to the actual mouse's normalized vector once we have that capability
			RigidBodyRef->AddForce(MouseVector * ChargeFactor * CurrentCharge);
			Charging = false;
			CurrentCharge = 0;
			CurrentlyFired = true;
			trace << "BallController::OnMouseUpEvent - ";
			if (event->ButtonReleased == MouseButton::Left)
				trace << "Left Button ";
			else if (event->ButtonReleased == MouseButton::Right)
				trace << "Right Button ";

			trace << "released at x: " << event->Position.x << " y: " << event->Position.y << "\n";
			
		}
       

	}

	void BallController::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{
		if (CurrentlyFired && Daisy->getMouse()->MouseDown(MouseButton::Left))
		{
			Vec3 CenteringVector = -glm::normalize(TransformRef->Translation);
			RigidBodyRef->AddForce(CenteringVector * 200.0f);
			//SpriteRef->Color = Vec4(1, 1, 1, 1);
		}
		if (Charging)
		{
			CurrentCharge += event->Dt;
			//trace << CurrentCharge << "\n";
			if (CurrentCharge > MaxCharge)
			{
				CurrentCharge = MaxCharge;
			}
		}
		//PrintVelocity();
		if (glm::distance(Vec3(0, -7, 0), TransformRef->Translation) < 6)
		{
			CurrentlyFired = false;
		}
		else
		{
			CurrentlyFired = true;
		}
	}



	void BallController::PrintTranslation()
	{
		//trace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x
		//	<< ", " << TransformRef->Translation.y
		//	<< ", " << TransformRef->Translation.z << ")\n";
	}

	void BallController::PrintVelocity()
	{
		Vec3 vel = RigidBodyRef->getVelocity();
		//trace << Owner()->Name() << "::RigidBody.Velocity(" << vel.x << ", " << vel.y<< ", " << vel.z << ")\n";
	}
}