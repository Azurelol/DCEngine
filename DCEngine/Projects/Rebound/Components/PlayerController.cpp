/******************************************************************************/
/*!
@file   PlayerController.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/23/2015
@brief  Controller for the main player! Handles everything the player does.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

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

	}

	void PlayerController::OnCollisionStartedEvent(Events::CollisionStarted * event)
	{
		Grounded = true;
	}

	void PlayerController::OnCollisionEndedEvent(Events::CollisionEnded * event)
	{
		//Grounded = false;
		if (PlayerControllerTraceOn)
		{
			DCTrace << "PlayerController::OnCollisionEndedEvent \n";
		}
	}

	void PlayerController::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{
		//DCTrace << "Grounded =" << Grounded << "\n";
		if (!Grounded)
		{
			SpriteComponent->SpriteSource = "MonkeyJump1";
			SpriteComponent->HaveAnimation = false;
			SpriteComponent->AnimationActive = false;
		}

		if (Daisy->getKeyboard()->KeyIsDown(Keys::W) || Daisy->getKeyboard()->KeyIsDown(Keys::Space))
		{
			if (Grounded)
			{
				Jump();
				Jumping = true;
				Grounded = false;
			}
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

		if (Daisy->getKeyboard()->KeyIsDown(Keys::A))
		{
			SpriteComponent->FlipX = true;
			MoveLeft();

		}

		if (Daisy->getKeyboard()->KeyIsDown(Keys::D))
		{
			SpriteComponent->FlipX = false;
			MoveRight();
		}
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

	void PlayerController::TakeDamage(int damage)
	{
		Health -= damage;
		if (PlayerControllerTraceOn)
		{
			DCTrace << "PlayerController::TakeDamage:: Health = " << Health << ".\n";
		}
		//dispatch damage taken event
		if (Health <= 0)
		{
			Die();
		}

	}

	void PlayerController::Die()
	{
	}



	void PlayerController::PrintTranslation()
	{
		DCTrace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x << ", " << TransformRef->Translation.y << ", " << TransformRef->Translation.z << ")\n";
	}

	void PlayerController::PrintVelocity()
	{
		Vec3 vel = RigidBodyRef->getVelocity();
		//DCTrace << Owner()->Name() << "::RigidBody.Velocity(" << vel.x << ", " << vel.y<< ", " << vel.z << ")\n";
	}

	void PlayerController::AutoPlay(Events::LogicUpdate * event)
	{
		switch (rand() % 3)
		{

		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		}
	}
	void PlayerController::MoveLeft()
	{
		if (RigidBodyRef->getVelocity().x > 0)
		{
			auto scalar = TurnSpeedScalar;
		}
		else
		{
			auto scalar = 1;
		}
		RigidBodyRef->ApplyLinearVelocity(Vec3(-MoveSpeed, 0, 0));


		//PrintTranslation();
	}
	void PlayerController::MoveRight()
	{
		if (RigidBodyRef->getVelocity().x < 0)
		{
			auto scalar = TurnSpeedScalar;
		}
		else
		{
			auto scalar = 1;
		}
		RigidBodyRef->ApplyLinearVelocity(Vec3(MoveSpeed, 0, 0));
		//PrintTranslation();
	}

	/**************************************************************************/
	/*!
	@brief Provides the definition of this class to Zilch.
	@note This can only go in the translational unit (.cpp)
	*/
	/**************************************************************************/
	#if(DCE_USE_ZILCH_INTERNAL_BINDING)
		ZilchDefineType(PlayerController, "PlayerController", DCEngineCore, builder, type) {
			// Constructor / Destructor
			ZilchBindConstructor(builder, type, PlayerController, "owner", Entity&);
			ZilchBindDestructor(builder, type, PlayerController);
			// Properties
		}
	#endif
}