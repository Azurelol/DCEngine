#pragma once
#include "../../ProjectComponent.h"

namespace DCEngine {

	class Transform;
	class RigidBody;
	class PlayerController : public Component {

	public:

		bool Translation = true;
		bool Grounded = true;
		Real MoveSpeed = 1;
		Real JumpPower = 60;
		Real AirBrakeScalar = 0.8f;
		Transform* TransformRef;
		RigidBody* RigidBodyRef;

		PlayerController(Entity& owner) : Component(std::string("PlayerController"), owner) {}
		void Initialize();
		virtual void Serialize(Json::Value& root);
		virtual void Deserialize(Json::Value& root);
		void OnMouseDownEvent(Events::MouseDown* event);
		void OnMouseUpEvent(Events::MouseUp* event);
		void OnKeyDownEvent(Events::KeyDown* event);
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