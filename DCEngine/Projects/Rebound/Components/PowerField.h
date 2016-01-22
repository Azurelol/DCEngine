/*****************************************************************************/
/*!
@file   PowerField.h
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"

namespace DCEngine {
	namespace Components
	{
		class Transform;
		class RigidBody;
		class Sprite;
		class PowerField : public Component {

		public:
			Transform* TransformRef;
			Sprite* SpriteRef;

			DCE_DEFINE_PROPERTY(String, Target);
			// Properties

			// Methods
			PowerField(Entity& owner) : Component(std::string("PowerField"), owner) {}
			void Initialize();
			void OnMouseDownEvent(Events::MouseDown* event);
			void OnMouseUpEvent(Events::MouseUp* event);
			void OnKeyDownEvent(Events::KeyDown* event);
			void OnKeyUpEvent(Events::KeyDown* event);
			void OnCollisionStartedEvent(Events::CollisionStarted* event);
			void OnCollisionEndedEvent(Events::CollisionEnded* event);
			void OnLogicUpdateEvent(Events::LogicUpdate * event);

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
			ZilchDeclareDerivedType(PowerField, Component);
#endif

		private:
			// Member variables
			String Target;
		};
	}

}