/******************************************************************************/
/*!
@file   EnemyController.h
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/23/2015
@brief  Header for EnemyController
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"

namespace DCEngine {

	class Transform;
	class RigidBody;
	class Sprite;
	enum class EnemyType { RandomJumper, BasicChaser, AdvancedChaser};
	class EnemyController : public Component {

	public:

		bool Grounded = true;
		bool LockedOnPlayer = false;
		bool HitEndOfPatrol = false;
		Real MoveSpeed = 5.0f;
		Real JumpPowerY = 20.0f;
		Real JumpPowerX = 3.0f;
		Real JumpInterval = 2.0f;
		Real Timer = 0;
		Real PatrolRange = 15;
		Vec3 InitialPosition;
		EnemyType EnemyType = EnemyType::BasicChaser;
		String PlayerName = "Player";
		
    // Properties
    DCE_DEFINE_PROPERTY(Real, MoveSpeed);
    DCE_DEFINE_PROPERTY(Real, JumpPowerY);
    DCE_DEFINE_PROPERTY(Real, JumpPowerX);
    DCE_DEFINE_PROPERTY(Real, JumpInterval);
    DCE_DEFINE_PROPERTY(Real, PatrolRange);    
    DCE_DEFINE_PROPERTY(Real, Timer);
	DCE_DEFINE_PROPERTY(String, PlayerName);
    
    // Methods
		EnemyController(Entity& owner) : Component(std::string("EnemyController"), owner) {}
		void Initialize();
		virtual void Serialize(Json::Value& root);
		virtual void Deserialize(Json::Value& root);
		void OnCollisionStartedEvent(Events::CollisionStarted* event);
		void OnCollisionEndedEvent(Events::CollisionEnded* event);
		void OnLogicUpdateEvent(Events::LogicUpdate * event);
		void Patrol();
		void DoBasicChaser();

		#if (DCE_USE_ZILCH_INTERNAL_BINDING)
	  ZilchDeclareDerivedType(EnemyController, Component);
		#endif

	private:
    Transform* TransformRef;
    RigidBody* RigidBodyRef;
    Sprite* SpriteRef;
    GameObject* PlayerRef;

		void PrintTranslation();
		void PrintVelocity();
		std::string FootstepSound;
		Boolean FootstepSoundEnabled = false;
	};


}