#include "BallController.h"
#include "../../CoreComponents.h"

namespace DCEngine {

	void BallController::Initialize()
	{
		auto gameObj = dynamic_cast<GameObject*>(Owner());
		Connect(Daisy->getMouse(), Events::MouseDown, BallController::OnMouseDownEvent);
		Connect(Daisy->getMouse(), Events::MouseUp, BallController::OnMouseUpEvent);
		Connect(gameObj, Events::CollisionStarted, BallController::OnCollisionStartedEvent);
		Connect(gameObj, Events::CollisionEnded, BallController::OnCollisionEndedEvent);
		Connect(SpaceRef, Events::LogicUpdate, BallController::OnLogicUpdateEvent);
		TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Transform>(); // ew
		RigidBodyRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<RigidBody>();
		SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Sprite>();
		PlayerRef = SpaceRef->FindObjectByName("Mariah");
		DCTrace << PlayerRef->getComponent<Transform>()->Translation.x;
	}

	void BallController::Serialize(Json::Value & root)
	{
	}

	void BallController::Deserialize(Json::Value & root)
	{
	}

	void BallController::OnMouseDownEvent(Events::MouseDown * event) 
	{

    // Call the CameraViewport component of the space with this screen position.. ?


    //DCTrace << "BallController::OnMouseDownEvent - Mouse Pos (Pixels): " 
    //      << event->Position.x << " y: " << event->Position.y << "\n";
    //DCTrace << "BallController::OnMouseDownEvent - Mouse Pos (World): "
    //  << coords.x << " y: " << coords.y << "\n";

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
			auto coords = SpaceRef->getComponent<CameraViewport>()->ScreenToViewport(Vec2(event->Position));
			DCTrace << "Coords =" << coords.x << ", " << coords.y << ").\n";
			DCTrace << "PlayerPos =" << PlayerRef->getComponent<Transform>()->Translation.x << ", " << PlayerRef->getComponent<Transform>()->Translation.y << ").\n";
			auto MouseVector = glm::normalize(Vec3(coords.x - PlayerRef->getComponent<Transform>()->Translation.x, coords.y - PlayerRef->getComponent<Transform>()->Translation.y, 0));
			if (CurrentCharge < MinCharge)
			{
				CurrentCharge = MinCharge;
			}
			
			RigidBodyRef->AddForce(MouseVector * ChargeFactor * CurrentCharge);
			DCTrace << "Vector of ball shot = (" << MouseVector.x << ", " << MouseVector.y << ", " << MouseVector.z << ").\n";
			Charging = false;
			CurrentCharge = 0;
			CurrentlyFired = true;
			SpriteRef->Color = Vec4(1, 0, 0, 1);
			RigidBodyRef->setGravityRatio(0.1);

			//DCTrace << "BallController::OnMouseUpEvent - ";
			//if (event->ButtonReleased == MouseButton::Left)
			//	DCTrace << "Left Button ";
			//else if (event->ButtonReleased == MouseButton::Right)
			//	DCTrace << "Right Button ";

			DCTrace << "released at x: " << event->Position.x << " y: " << event->Position.y << "\n";
			
		}
       

	}

	void BallController::OnCollisionStartedEvent(Events::CollisionStarted * event)
	{
		RigidBodyRef->setGravityRatio(1.0f);
		if (event->OtherObject->getComponent<PlayerController>())
		{
			CurrentlyFired = false;
			SpriteRef->Color = Vec4(0, 1, 0, 1);
		}
	}

	void BallController::OnCollisionEndedEvent(Events::CollisionEnded * event)
	{
	}

	void BallController::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{
		SpriteRef->Color = SpriteRef->getColor() + Vec4(0, 0, CurrentCharge / MaxCharge, 0);
		if (CurrentlyFired && Daisy->getMouse()->MouseDown(MouseButton::Left))
		{
			Vec3 CenteringVector = glm::normalize(PlayerRef->getComponent<Transform>()->Translation - TransformRef->Translation);
			RigidBodyRef->AddForce(CenteringVector * 200.0f);

		}
		if (Charging)
		{
			CurrentCharge += event->Dt;
			//DCTrace << CurrentCharge << "\n";
			if (CurrentCharge > MaxCharge)
			{
				CurrentCharge = MaxCharge;
			}
		}
		//PrintVelocity();
	}



	void BallController::PrintTranslation()
	{
		//DCTrace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x
		//	<< ", " << TransformRef->Translation.y
		//	<< ", " << TransformRef->Translation.z << ")\n";
	}

	void BallController::PrintVelocity()
	{
		Vec3 vel = RigidBodyRef->getVelocity();
		//DCTrace << Owner()->Name() << "::RigidBody.Velocity(" << vel.x << ", " << vel.y<< ", " << vel.z << ")\n";
	}
}