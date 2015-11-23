#pragma once
#include "../../ProjectComponent.h"

namespace DCEngine {

	class Transform;
	class RigidBody;
	class Sprite;
	class HazardArea : public Component {

	public:

		Real Damage = 1.0f;
		Real DamageInterval = .0f;
		Real Timer = DamageInterval; //will always damage the player on first tick of contact
		bool TouchingPlayer = true;
		Transform* TransformRef;
		RigidBody* RigidBodyRef;
		Sprite* SpriteRef;
		GameObject* PlayerRef;

		HazardArea(Entity& owner) : Component(std::string("HazardArea"), owner) {}
		void Initialize();
		virtual void Serialize(Json::Value& root);
		virtual void Deserialize(Json::Value& root);
		void OnCollisionStartedEvent(Events::CollisionStarted* event);
		void OnCollisionEndedEvent(Events::CollisionEnded* event);
		void OnLogicUpdateEvent(Events::LogicUpdate * event);

		#if (DCE_USE_ZILCH_INTERNAL_BINDING)
				ZilchDeclareDerivedType(HazardArea, Component);
		#endif

	private:
		void PrintTranslation();
		void PrintVelocity();
		std::string FootstepSound;
		Boolean FootstepSoundEnabled = false;
	};


}