#pragma once
#include "../../ProjectComponent.h"
#define SkeletonTraceOn 0

namespace DCEngine {

	class Transform;
	class Sprite;
	class RigidBody;
	class Skeleton : public Component {

	public:
		Transform* TransformRef;
		Sprite* SpriteRef;
		RigidBody* RigidBodyRef;

		Skeleton(Entity& owner) : Component(std::string("Skeleton"), owner) {}
		void Initialize();
		virtual void Serialize(Json::Value& root);
		virtual void Deserialize(Json::Value& root);
		void OnMouseDownEvent(Events::MouseDown* event);
		void OnMouseUpEvent(Events::MouseUp* event);
		void OnKeyDownEvent(Events::KeyDown* event);
		void OnKeyUpEvent(Events::KeyDown* event);
		void OnCollisionStartedEvent(Events::CollisionStarted* event);
		void OnCollisionEndedEvent(Events::CollisionEnded* event);
		void Skeleton::OnLogicUpdateEvent(Events::LogicUpdate * event);

		#if (DCE_USE_ZILCH_INTERNAL_BINDING)
				ZilchDeclareDerivedType(Skeleton, Component);
		#endif

	private:
		void PrintTranslation();
	};

}