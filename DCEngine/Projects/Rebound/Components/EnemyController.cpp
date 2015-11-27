/******************************************************************************/
/*!
@file   EnemyController.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/23/2015
@brief  Controller for all the different types of enemies.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "EnemyController.h"
#include "../../CoreComponents.h"

namespace DCEngine {

	void EnemyController::Initialize()
	{
		auto gameObj = dynamic_cast<GameObject*>(Owner());
		Connect(gameObj, Events::CollisionStarted, EnemyController::OnCollisionStartedEvent);
		Connect(gameObj, Events::CollisionEnded, EnemyController::OnCollisionEndedEvent);
		Connect(SpaceRef, Events::LogicUpdate, EnemyController::OnLogicUpdateEvent);
		TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Transform>(); // ew
		RigidBodyRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<RigidBody>();
		SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Sprite>();
		PlayerRef = SpaceRef->FindObjectByName("Mariah");
		InitialPosition = TransformRef->getTranslation();
	}

	void EnemyController::Serialize(Json::Value & root)
	{
	}

	void EnemyController::Deserialize(Json::Value & root)
	{
	}

	void EnemyController::OnCollisionStartedEvent(Events::CollisionStarted * event)
	{
		if (event->OtherObject->getComponent<BallController>())
		{
			//TransformRef->Translation.x = -1000;
			//TransformRef->Translation.y = -1000;
		}
	}

	void EnemyController::OnCollisionEndedEvent(Events::CollisionEnded * event)
	{
	}

	void EnemyController::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{
		Timer += event->Dt;
		if (EnemyType == EnemyType::BasicChaser)
		{
			DoBasicChaser();
			Patrol();
		}
		if (EnemyType == EnemyType::RandomJumper)
		{
			if (Timer > JumpInterval)
			{
				Timer = 0;
				auto direction = rand() % 11 - 5; //-5 to 5
				RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(direction * JumpPowerX, JumpPowerY, 0));
			}
		}
	}

	void EnemyController::Patrol()
	{
		auto XDistanceFromPlayer = TransformRef->Translation.x - PlayerRef->getComponent<Transform>()->Translation.x;
		auto direction = glm::sign(XDistanceFromPlayer); //-5 to 5
		if(LockedOnPlayer)
		{
			if (direction < 0)
			{
				RigidBodyRef->setVelocity(Vec3(MoveSpeed, 0, 0));
			}
			if (direction > 0)
			{
				RigidBodyRef->setVelocity(Vec3(-MoveSpeed, 0, 0));
			}
		}

		if (TransformRef->getTranslation().x < InitialPosition.x)
		{
			RigidBodyRef->setVelocity(Vec3(MoveSpeed, 0, 0));
			HitEndOfPatrol = false;
		}
		else if (TransformRef->getTranslation().x > InitialPosition.x + PatrolRange)
		{
			RigidBodyRef->setVelocity(Vec3(-MoveSpeed, 0, 0));
			HitEndOfPatrol = true;
		}
		else
		{
			if (HitEndOfPatrol && !LockedOnPlayer)
			{
				RigidBodyRef->setVelocity(Vec3(-MoveSpeed, 0, 0));
			}
			if (!HitEndOfPatrol && !LockedOnPlayer)
			{
				RigidBodyRef->setVelocity(Vec3(MoveSpeed, 0, 0));
			}
		}
		
	

		if (RigidBodyRef->getVelocity().x < 0)
		{
			SpriteRef->FlipX = true;
		}
		else
		{
			SpriteRef->FlipX = false;
		}
		
	}

	void EnemyController::DoBasicChaser()
	{
		auto PlayerPosX = PlayerRef->getComponent<Transform>()->Translation.x;
		if (PlayerPosX > InitialPosition.x && PlayerPosX < InitialPosition.x + PatrolRange)
		{
			LockedOnPlayer = true;
			if (Timer > JumpInterval)
			{
				//Timer = 0;
				//RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(direction * JumpPowerX, JumpPowerY, 0));
			}
			SpriteRef->Color = Vec4(1, 0, 1, 1);
		}
		else
		{
			SpriteRef->Color = Vec4(0, 0, 1, 1);
			LockedOnPlayer = false;
		}
	}

	Real EnemyController::GetJumpInterval()
	{
		return JumpInterval;
	}

	void EnemyController::SetJumpInterval(Real val)
	{
		JumpInterval = val;
	}

	Real EnemyController::GetJumpPowerY()
	{
		return JumpPowerY;
	}

	void EnemyController::SetJumpPowerY(Real val)
	{
		JumpPowerY = val;
	}

	Real EnemyController::GetJumpPowerX()
	{
		return Real();
	}

	void EnemyController::SetJumpPowerX(Real val)
	{
	}

	Real EnemyController::GetMoveSpeed()
	{
		return MoveSpeed;
	}

	void EnemyController::SetMoveSpeed(Real val)
	{
		MoveSpeed = val;
	}



	void EnemyController::PrintTranslation()
	{
		DCTrace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x << ", " << TransformRef->Translation.y << ", " << TransformRef->Translation.z << ")\n";
	}

	void EnemyController::PrintVelocity()
	{
		Vec3 vel = RigidBodyRef->getVelocity();
		//DCTrace << Owner()->Name() << "::RigidBody.Velocity(" << vel.x << ", " << vel.y<< ", " << vel.z << ")\n";
	}
	/**************************************************************************/
	/*!
	@brief Provides the definition of this class to Zilch.
	@note This can only go in the translational unit (.cpp)
	*/
	/**************************************************************************/
	#if(DCE_USE_ZILCH_INTERNAL_BINDING)
		ZilchDefineType(EnemyController, "EnemyController", Rebound, builder, type) {
      REBOUND_BINDING_INTERNAL_COMPONENT_SET_HANDLE_TYPE;
			// Constructor / Destructor
			ZilchBindConstructor(builder, type, EnemyController, "owner", Entity&);
			ZilchBindDestructor(builder, type, EnemyController);
			// Properties
			ZilchBindProperty(builder, type, &EnemyController::GetMoveSpeed, &EnemyController::SetMoveSpeed, "MoveSpeed");
			ZilchBindProperty(builder, type, &EnemyController::GetJumpInterval, &EnemyController::SetJumpInterval, "JumpInterval");
			ZilchBindProperty(builder, type, &EnemyController::GetJumpPowerY, &EnemyController::SetJumpPowerY, "JumpPowerY");
			ZilchBindProperty(builder, type, &EnemyController::GetJumpPowerX, &EnemyController::SetJumpPowerX, "JumpPowerX");
		}
	#endif
}