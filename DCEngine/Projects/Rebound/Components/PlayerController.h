#pragma once
#include "../../ProjectComponent.h"

namespace DCEngine {

	class Transform;
	class RigidBody;
	class PlayerController : public Component {

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

		PlayerController(Entity& owner) : Component(std::string("PlayerController"), owner) {}
		void Initialize();
		virtual void Serialize(Json::Value& root);
		virtual void Deserialize(Json::Value& root);
		void OnMouseDownEvent(Events::MouseDown* event);
		void OnMouseUpEvent(Events::MouseUp* event);
		void PlayerController::OnLogicUpdateEvent(Events::LogicUpdate * event);

	private:
		void PrintTranslation();
		void PrintVelocity();
		std::string FootstepSound;
		Boolean FootstepSoundEnabled = false;
	};


}