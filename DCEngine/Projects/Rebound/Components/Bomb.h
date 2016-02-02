/*****************************************************************************/
/*!
@file   Bomb.h
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"

namespace DCEngine {
	namespace Components {
		class Transform;
		class RigidBody;
		class Sprite;
		class Bomb : public Component {

		public:
			Transform* TransformRef;
			RigidBody* RigidBodyRef;
			Sprite* SpriteRef;
			Real FuseLength = 5;
			Boolean TimerStarted = false;

			// Properties
			DCE_DEFINE_PROPERTY(Real, Timer);
			DCE_DEFINE_PROPERTY(Real, FuseLength);



			// Methods
			Bomb(Entity& owner) : Component(std::string("Bomb"), owner) {}
			void Initialize();
			void OnMouseDownEvent(Events::MouseDown* event);
			void OnMouseUpEvent(Events::MouseUp* event);
			void OnKeyDownEvent(Events::KeyDown* event);
			void OnKeyUpEvent(Events::KeyDown* event);
			void OnCollisionStartedEvent(Events::CollisionStarted* event);
			void OnCollisionEndedEvent(Events::CollisionEnded* event);
			void OnCollisionPersistedEvent(Events::CollisionPersisted * event);
			void OnLogicUpdateEvent(Events::LogicUpdate * event);
			void Explode();

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
			ZilchDeclareDerivedType(Bomb, Component);
#endif

		private:
			// Member variables
			Real Timer = 0;
		};
	}

}