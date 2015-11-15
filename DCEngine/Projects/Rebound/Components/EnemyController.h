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

	private:
		void PrintTranslation();
		void PrintVelocity();
		std::string FootstepSound;
		Boolean FootstepSoundEnabled = false;
	};


}