#pragma once
#include "../../ProjectComponent.h"

namespace DCEngine {

	class Transform;
	class RigidBody;
	class BallController : public Component {

	public:

		bool Translation = true;
		bool CurrentlyFired = false;
		Real MoveSpeed = 0.75;
		Real RotSpeed = 15;
		Transform* TransformRef;
		RigidBody* RigidBodyRef;
//		Sprite* SpriteRef;
		Real CurrentCharge = 1;
		Real MaxCharge = 2;
		Real ChargeFactor = 80;
		Boolean Charging = false;
		GameObject* PlayerRef;

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
	};


}