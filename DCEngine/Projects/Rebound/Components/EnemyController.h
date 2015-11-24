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
#include "../../ProjectComponent.h"

namespace DCEngine {

	class Transform;
	class RigidBody;
	class Sprite;
	class EnemyController : public Component {

	public:

		bool Grounded = true;
		Real MoveSpeed = 3.0f;
		Real JumpPower = 20.0f;
		Real JumpInterval = 2.0f;
		Real Timer = 0;
		Transform* TransformRef;
		RigidBody* RigidBodyRef;
		Sprite* SpriteRef;
		GameObject* PlayerRef;

		EnemyController(Entity& owner) : Component(std::string("EnemyController"), owner) {}
		void Initialize();
		virtual void Serialize(Json::Value& root);
		virtual void Deserialize(Json::Value& root);
		void OnCollisionStartedEvent(Events::CollisionStarted* event);
		void OnCollisionEndedEvent(Events::CollisionEnded* event);
		void OnLogicUpdateEvent(Events::LogicUpdate * event);

		#if (DCE_USE_ZILCH_INTERNAL_BINDING)
			ZilchDeclareDerivedType(EnemyController, Component);
		#endif

	private:
		void PrintTranslation();
		void PrintVelocity();
		std::string FootstepSound;
		Boolean FootstepSoundEnabled = false;
	};


}