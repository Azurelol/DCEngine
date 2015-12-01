/******************************************************************************/
/*!
@file   PlayerController.h
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/23/2015
@brief  Header for PlayerController
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"


#define PlayerControllerTraceOn 0 // Use a static bool inside the class

namespace DCEngine {

  class Sprite;
	class Transform;
	class RigidBody;
	class PlayerController : public Component {

	public:
		bool Grounded = true;
		bool Jumping = false;
		bool DoAutoPlay = true;
		bool Invincible = true;
		Real MoveSpeed = 0.7f;
		Real JumpPower = 60.0f;
		Real JumpFrames = 1.0f;
		Real JumpFramesApplied = 0;
		Real Health = 10;
		Real AirBrakeScalar = 0.95f; //multiplies your current y velocity by this when you release jump while jumping upwards
		Real TurnSpeedScalar = 5.0f; //how much faster you accelerate when attemping to accelerate opposite your current velocity
		String StandAnimation = "PlayerStand";
		String JumpAnimation = "PlayerFall";
    
    
    Transform* TransformRef;
		RigidBody* RigidBodyRef;
    
    DCE_DEFINE_PROPERTY(Real, MoveSpeed);
    DCE_DEFINE_PROPERTY(Real, JumpPower);
    DCE_DEFINE_PROPERTY(Real, JumpFrames);
    DCE_DEFINE_PROPERTY(Real, Health);
    DCE_DEFINE_PROPERTY(Real, AirBrakeScalar);
    DCE_DEFINE_PROPERTY(Real, TurnSpeedScalar);
    DCE_DEFINE_PROPERTY(Boolean, DoAutoPlay);
    DCE_DEFINE_PROPERTY(String, StandAnimation);
    DCE_DEFINE_PROPERTY(String, JumpAnimation);

		PlayerController(Entity& owner) : Component(std::string("PlayerController"), owner) {}
		void Initialize();
		virtual void Serialize(Json::Value& root);
		virtual void Deserialize(Json::Value& root);
		void OnMouseDownEvent(Events::MouseDown* event);
		void OnMouseUpEvent(Events::MouseUp* event);
		void OnKeyDownEvent(Events::KeyDown* event);
		void OnCollisionStartedEvent(Events::CollisionStarted* event);
		void OnCollisionEndedEvent(Events::CollisionEnded* event);
    void OnCollisionPersistedEvent(Events::CollisionPersisted* event);
		void OnLogicUpdateEvent(Events::LogicUpdate * event);
		//void OnDamageEvent(Events::DamageEvent event);
		void Jump();
		void TakeDamage(int damage);
		void Die();
		#if (DCE_USE_ZILCH_INTERNAL_BINDING)
				ZilchDeclareDerivedType(PlayerController, Component);
		#endif

	private:
    Sprite* SpriteComponent;
		void PrintTranslation();
		void PrintVelocity();
		void AutoPlay(Events::LogicUpdate * event);
		void MoveLeft();
		void MoveRight();
		std::string FootstepSound;
		Boolean FootstepSoundEnabled = false;
	};


}