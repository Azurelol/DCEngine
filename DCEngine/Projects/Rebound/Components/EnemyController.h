#pragma once
#include "../../ProjectComponent.h"

namespace DCEngine {

	class Transform;
	class RigidBody;
	class EnemyController : public Component {

	public:

		bool Translation = true;
		bool Grounded = true;
		Real MoveSpeed = 0.6f;
		Real JumpPower = 1.0f;
		Real AirBrakeScalar = 0.8f;
		Transform* TransformRef;
		RigidBody* RigidBodyRef;

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