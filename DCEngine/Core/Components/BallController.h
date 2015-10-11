#pragma once
#include "ComponentReference.h"

namespace DCEngine {

	class Transform;
	class RigidBody;
	class BallController : public Component {

	public:

		bool Translation = true;
		Real MoveSpeed = 0.75;
		Real RotSpeed = 15;
		Transform* TransformRef;
		RigidBody* RigidBodyRef;
		Real CurrentCharge = 1;
		Real MaxCharge = 1;
		Real ChargeFactor = 10000;
		Boolean Charging = false;

		BallController(Entity& owner) : Component(std::string("BallController"), owner) {}
		void Initialize();
		virtual void Serialize(Json::Value& root);
		virtual void Deserialize(Json::Value& root);
		void OnMouseDownEvent(Events::MouseDown* event);
		void OnMouseUpEvent(Events::MouseUp* event);
		void BallController::OnLogicUpdateEvent(Events::LogicUpdate * event);

	private:
		void PrintTranslation();
		void PrintVelocity();
		std::string FootstepSound;
		Boolean FootstepSoundEnabled = false;
	};


}