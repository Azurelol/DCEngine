#include "PlayerController.h"
#include "../../CoreComponents.h"


namespace DCEngine {

	void PlayerController::Initialize()
	{
		auto gameObj = dynamic_cast<GameObject*>(Owner());
		Connect(Daisy->getMouse(), Events::MouseDown, PlayerController::OnMouseDownEvent);
		Connect(Daisy->getMouse(), Events::MouseUp, PlayerController::OnMouseUpEvent);
		Connect(Daisy->getKeyboard(), Events::KeyDown, PlayerController::OnKeyDownEvent);
		Connect(gameObj, Events::CollisionStarted, PlayerController::OnCollisionStartedEvent);
		Connect(gameObj, Events::CollisionEnded, PlayerController::OnCollisionEndedEvent);
		Connect(SpaceRef, Events::LogicUpdate, PlayerController::OnLogicUpdateEvent);
		TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Transform>(); // ew
		RigidBodyRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<RigidBody>();
    SpriteComponent = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Sprite>();
	}

	void PlayerController::Serialize(Json::Value & root)
	{
	}

	void PlayerController::Deserialize(Json::Value & root)
	{
	}

	void PlayerController::OnMouseDownEvent(Events::MouseDown * event)
	{

	}
	void PlayerController::OnMouseUpEvent(Events::MouseUp * event)
	{

	}

	void PlayerController::OnKeyDownEvent(Events::KeyDown* event) 
	{
		switch (event->Key) 
		{

		case Keys::W:
			Jumping = true;
			Grounded = false;
			//RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(0, JumpPower, 0));
			PrintTranslation();
			break;
		case Keys::A:
			RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(-MoveSpeed, 0, 0));
			PrintTranslation();
			break;
		case Keys::D:
			RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(MoveSpeed, 0, 0));
			PrintTranslation();
			break;
		}
	}

	void PlayerController::OnCollisionStartedEvent(Events::CollisionStarted * event)
	{
		Grounded = true;
	}

	void PlayerController::OnCollisionEndedEvent(Events::CollisionEnded * event)
	{
		Grounded = false;
	}

	void PlayerController::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{
		if (Jumping)
		{
      SpriteComponent->SpriteSource = "MonkeyJump1";
      SpriteComponent->HaveAnimation = false;
      SpriteComponent->AnimationActive = false;
			Jump();
		}
		if (Daisy->getKeyboard()->KeyIsDown(Keys::W))
		{

		}
		else
		{
      SpriteComponent->SpriteSource = "MonkeyWalk1";
      SpriteComponent->HaveAnimation = true;
      SpriteComponent->AnimationActive = true;
			Jumping = false;
			if (RigidBodyRef->getVelocity().y > 0)
			{
				RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() * Vec3(1, AirBrakeScalar, 1));
			}
		}
		//if (Daisy->getKeyboard()->KeyIsDown(Keys::W) && Grounded)
		//{
			//RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(0, JumpPower, 0));
			//Grounded = false;
		//}
		//else if (!Grounded && RigidBodyRef->getVelocity().y > 0)
		//{
		//	RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() * Vec3(1, AirBrakeScalar, 1));
		//}

		if (Daisy->getKeyboard()->KeyIsDown(Keys::A))
		{
			RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(-MoveSpeed, 0, 0));
			//DCTrace << "AAA";
		}

		if (Daisy->getKeyboard()->KeyIsDown(Keys::D))
		{
			RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(MoveSpeed, 0, 0));
		}
		//if (Daisy->getKeyboard()->KeyIsDown(Keys::Space))
		//{
		//	RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(MoveSpeed, 0, 0));
		//	//DCTrace << "Space";
		//}
	}

	void PlayerController::Jump()
	{
		++JumpFramesApplied;
		RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(0, JumpPower, 0));
		if (JumpFramesApplied >= JumpFrames)
		{
			Jumping = false;
			JumpFramesApplied = 0;
		}
		
	}



	void PlayerController::PrintTranslation()
	{
		DCTrace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x	<< ", " << TransformRef->Translation.y << ", " << TransformRef->Translation.z << ")\n";
	}

	void PlayerController::PrintVelocity()
	{
		Vec3 vel = RigidBodyRef->getVelocity();
		//DCTrace << Owner()->Name() << "::RigidBody.Velocity(" << vel.x << ", " << vel.y<< ", " << vel.z << ")\n";
	}
}