/*****************************************************************************/
/*!
@file   Skeleton.h
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"

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