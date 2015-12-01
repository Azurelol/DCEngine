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

  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(PlayerController, "PlayerController", Rebound, builder, type) {    
    DCE_BINDING_DEFINE_PROPERTY(PlayerController, MoveSpeed);
    DCE_BINDING_DEFINE_PROPERTY(PlayerController, JumpPower);
    DCE_BINDING_DEFINE_PROPERTY(PlayerController, JumpFrames);
    DCE_BINDING_DEFINE_PROPERTY(PlayerController, Health);
    DCE_BINDING_DEFINE_PROPERTY(PlayerController, AirBrakeScalar);
    DCE_BINDING_DEFINE_PROPERTY(PlayerController, TurnSpeedScalar);
    DCE_BINDING_DEFINE_PROPERTY(PlayerController, DoAutoPlay);
    DCE_BINDING_DEFINE_PROPERTY(PlayerController, StandAnimation);
    DCE_BINDING_DEFINE_PROPERTY(PlayerController, JumpAnimation);
  }
  #endif

	void PlayerController::Initialize()
	{
		auto gameObj = dynamic_cast<GameObject*>(Owner());
		Connect(Daisy->getMouse(), Events::MouseDown, PlayerController::OnMouseDownEvent);
		Connect(Daisy->getMouse(), Events::MouseUp, PlayerController::OnMouseUpEvent);
		Connect(Daisy->getKeyboard(), Events::KeyDown, PlayerController::OnKeyDownEvent);
		Connect(gameObj, Events::CollisionStarted, PlayerController::OnCollisionStartedEvent);
		Connect(gameObj, Events::CollisionEnded, PlayerController::OnCollisionEndedEvent);
    Connect(gameObj, Events::CollisionPersisted, PlayerController::OnCollisionPersistedEvent);
		Connect(SpaceRef, Events::LogicUpdate, PlayerController::OnLogicUpdateEvent);
		//Connect(gameObj, Events::DamageEvent, PlayerController::OnDamageEvent);

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
		if (LevelCheatLoaded = true)
		{
			return;
		}

		String level;
		switch (event->Key)
		{
		case Keys::I:
			Invincible = !Invincible;
			if (Invincible)
			{
				SpriteComponent->Color = Vec4(1, 1, 0, 1);
			}
			else
			{
				SpriteComponent->Color = Vec4(1, 1, 1, 1);
			}
			break;
		case Keys::O:
			LevelCheatLoaded = true;
			Die();
			break;
		case Keys::P:
			LevelCheatLoaded = true;
			level = "YouWon";
			SpaceRef->LoadLevel(level);
			break;
		case Keys::Num1:
			LevelCheatLoaded = true;
			level = "Level1";
			SpaceRef->LoadLevel(level);
			break;
		case Keys::Num2:
			LevelCheatLoaded = true;
			level = "Level2";
			SpaceRef->LoadLevel(level);
			break;
		case Keys::Num3:
			LevelCheatLoaded = true;
			level = "Level3";
			SpaceRef->LoadLevel(level);
			break;

		default:
			break;
		}
	}

	void PlayerController::OnCollisionStartedEvent(Events::CollisionStarted * event)
	{
		if (event->OtherObject->getComponent<Transform>()->getTranslation().y + event->OtherObject->getComponent<Transform>()->getScale().y / 2  < TransformRef->getTranslation().y)
		{
			Grounded = true;
			this->SpaceRef->getComponent<SoundSpace>()->PlayCue("HighThud");
		}
	}

	void PlayerController::OnCollisionEndedEvent(Events::CollisionEnded * event)
	{
		//Grounded = false;
		if (PlayerControllerTraceOn)
		{
			DCTrace << "PlayerController::OnCollisionEndedEvent \n";
		}
	}

  void PlayerController::OnCollisionPersistedEvent(Events::CollisionPersisted * event)
  {
    //DCTrace << "PlayerController::OnCollisionPersistedEvent - \n";
  }

	void PlayerController::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{
		//DCTrace << "Grounded =" << Grounded << "\n";
		if (!Grounded)
		{
			SpriteComponent->SpriteSource = JumpAnimation;
			//SpriteComponent->HaveAnimation = false;
			//SpriteComponent->AnimationActive = false;
      RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() * 0.99f);
		}
		else
		{
			SpriteComponent->SpriteSource = StandAnimation;
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
			//SpriteComponent->HaveAnimation = true;
			//SpriteComponent->AnimationActive = true;
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
      RigidBodyRef->setFriction(0.3f);
		}
		else if (Daisy->getKeyboard()->KeyIsDown(Keys::D))
		{
			SpriteComponent->FlipX = false;
			MoveRight();
      RigidBodyRef->setFriction(0.3f);
		}
    else
    {
      RigidBodyRef->setFriction(1.3f);
    }
	}

	//void PlayerController::OnDamageEvent(Events::DamageEvent * event)
	//{
	//}

	void PlayerController::Jump()
	{
		++JumpFramesApplied;
		RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(0, JumpPower, 0));
		if (JumpFramesApplied >= JumpFrames)
		{
			Jumping = false;
			JumpFramesApplied = 0;
		}
		this->SpaceRef->getComponent<SoundSpace>()->PlayCue("FootConcreteBootRun2");
	}

	void PlayerController::TakeDamage(int damage)
	{
		if(Invincible)
		{
			return;
		}

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
		DCTrace << "PlayerController::Die - Reloading level \n";
		this->SpaceRef->ReloadLevel();
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
    float scalar = 0.0f;

		if (RigidBodyRef->getVelocity().x > 0)
		{
			scalar = TurnSpeedScalar;
		}
		else
		{
			scalar = 1;
		}

    if (RigidBodyRef->getVelocity().length() < 4.0f)
    {
      RigidBodyRef->ApplyLinearVelocity(Vec3(-MoveSpeed * scalar * 1.5f, 0, 0));
    }
    else
    {
      RigidBodyRef->ApplyLinearVelocity(Vec3(-MoveSpeed * scalar, 0, 0));
    }

		//PrintTranslation();
	}
	void PlayerController::MoveRight()
	{
    float scalar = 0.0f;

		if (RigidBodyRef->getVelocity().x < 0)
		{
			scalar = TurnSpeedScalar;
		}
		else
		{
			scalar = 1;
		}
		//PrintTranslation();
    if (RigidBodyRef->getVelocity().length() < 4.0f)
    {
      RigidBodyRef->ApplyLinearVelocity(Vec3(MoveSpeed * scalar * 1.5f, 0, 0));
    }
    else
    {
      RigidBodyRef->ApplyLinearVelocity(Vec3(MoveSpeed * scalar, 0, 0));
    }

	}

}