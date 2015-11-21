#pragma once
#include "../../ProjectComponent.h"
#define BallControllerTraceOn 0
namespace DCEngine {

	class Transform;
	class RigidBody;
	class Sprite;
	class BallController : public Component {

	public:

		bool Translation = true;
		bool Frozen = false;
		bool CurrentlyFired = false;
		Real MoveSpeed = 0.75;
		Real RotSpeed = 15;
		Transform* TransformRef;
		RigidBody* RigidBodyRef;
		Sprite* SpriteRef;
		Real CurrentCharge = 0;
		Real MaxCharge = 2;
		Real MinCharge = 0.5f;
		Real ChargeFactor = 100 * 1000;
		Boolean Charging = false;
		Vec4 FrozenColor = Vec4(1, 0, 1, 1);
		Vec4 NormalColor = Vec4(0.0f, 0.7f, 0.3f, 1.0f);
		Vec4 ChargedColor = Vec4(0.0f, 0.7f, 1.0f, 1.0f);
		GameObject* PlayerRef;

		BallController(Entity& owner) : Component(std::string("BallController"), owner) {}
		void Initialize();
		virtual void Serialize(Json::Value& root);
		virtual void Deserialize(Json::Value& root);
		void OnMouseDownEvent(Events::MouseDown* event);
		void OnMouseUpEvent(Events::MouseUp* event);
		void OnCollisionStartedEvent(Events::CollisionStarted* event);
		void OnCollisionEndedEvent(Events::CollisionEnded* event);
		void BallController::OnLogicUpdateEvent(Events::LogicUpdate * event);
		void ChangeColor();

	private:
		void PrintTranslation();
		void PrintVelocity();
	};


}