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

  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(EnemyController, "EnemyController", Rebound, builder, type) {
    DCE_BINDING_DEFINE_PROPERTY(EnemyController, MoveSpeed);
    DCE_BINDING_DEFINE_PROPERTY(EnemyController, JumpPowerY);
    DCE_BINDING_DEFINE_PROPERTY(EnemyController, JumpPowerX);
    DCE_BINDING_DEFINE_PROPERTY(EnemyController, JumpInterval);
    DCE_BINDING_DEFINE_PROPERTY(EnemyController, PatrolRange);
    DCE_BINDING_DEFINE_PROPERTY(EnemyController, Timer);
  }
  #endif

	void EnemyController::Initialize()
	{
		auto gameObj = dynamic_cast<GameObject*>(Owner());
		Connect(gameObj, Events::CollisionStarted, EnemyController::OnCollisionStartedEvent);
		Connect(gameObj, Events::CollisionEnded, EnemyController::OnCollisionEndedEvent);
		Connect(SpaceRef, Events::LogicUpdate, EnemyController::OnLogicUpdateEvent);
		TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Transform>(); // ew
		RigidBodyRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<RigidBody>();
		SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Sprite>();
		PlayerRef = SpaceRef->FindObjectByName(PlayerName);
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
			dynamic_cast<GameObject*>(Owner())->Destroy();
		}
	}

	void EnemyController::OnCollisionEndedEvent(Events::CollisionEnded * event)
	{
	}

	void EnemyController::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{
		Timer += event->Dt;
		if (EnemyType == EnemyType::BasicChaser || EnemyType == EnemyType::AdvancedChaser)
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
				RigidBodyRef->setVelocity(Vec3(MoveSpeed, RigidBodyRef->getVelocity().y, 0));
			}
			if (direction > 0)
			{
				RigidBodyRef->setVelocity(Vec3(-MoveSpeed, RigidBodyRef->getVelocity().y, 0));
			}
		}

		if(EnemyType == EnemyType::AdvancedChaser)
		{
			return;
		}

		if (TransformRef->getTranslation().x < InitialPosition.x)
		{
			RigidBodyRef->setVelocity(Vec3(MoveSpeed, RigidBodyRef->getVelocity().y, 0));
			HitEndOfPatrol = false;
		}
		else if (TransformRef->getTranslation().x > InitialPosition.x + PatrolRange)
		{
			RigidBodyRef->setVelocity(Vec3(-MoveSpeed, RigidBodyRef->getVelocity().y, 0));
			HitEndOfPatrol = true;
		}
		else
		{
			if (HitEndOfPatrol && !LockedOnPlayer)
			{
				RigidBodyRef->setVelocity(Vec3(-MoveSpeed, RigidBodyRef->getVelocity().y, 0));
			}
			if (!HitEndOfPatrol && !LockedOnPlayer)
			{
				RigidBodyRef->setVelocity(Vec3(MoveSpeed, RigidBodyRef->getVelocity().y, 0));
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
  
	void EnemyController::PrintTranslation()
	{
		DCTrace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x << ", " << TransformRef->Translation.y << ", " << TransformRef->Translation.z << ")\n";
	}

	void EnemyController::PrintVelocity()
	{
		Vec3 vel = RigidBodyRef->getVelocity();
		//DCTrace << Owner()->Name() << "::RigidBody.Velocity(" << vel.x << ", " << vel.y<< ", " << vel.z << ")\n";
	}

}