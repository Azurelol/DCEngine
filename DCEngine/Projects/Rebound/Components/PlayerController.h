#pragma once
#include "../../ProjectComponent.h"

namespace DCEngine {

  class Sprite;
	class Transform;
	class RigidBody;
	class PlayerController : public Component {

	public:

		bool Translation = true;
		bool Grounded = true;
		bool Jumping = false;
		bool DoAutoPlay = true;
		Real MoveSpeed = 0.2f;
		Real JumpPower = 30.0f;
		Real JumpFrames = 1.0f;
		Real JumpFramesApplied = 0;
		Real AirBrakeScalar = 0.9f; //multiplies your current y velocity by this when you release jump while jumping upwards
		Real TurnSpeedScalar = 5.0f; //how much faster you accelerate when attemping to accelerate opposite your current velocity
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
		void Jump();

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