/*****************************************************************************/
/*!
@file   FreezeEnabler.h
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"

namespace DCEngine {

	class Transform;
	class RigidBody;
	class Sprite;
	class FreezeEnabler : public Component {

	public:
		Transform* TransformRef;
		Sprite* SpriteRef;

		// Properties

		// Methods
		FreezeEnabler(Entity& owner) : Component(std::string("FreezeEnabler"), owner) {}
		void Initialize();
		void OnMouseDownEvent(Events::MouseDown* event);
		void OnMouseUpEvent(Events::MouseUp* event);
		void OnKeyDownEvent(Events::KeyDown* event);
		void OnKeyUpEvent(Events::KeyDown* event);
		void OnCollisionStartedEvent(Events::CollisionStarted* event);
		void OnCollisionEndedEvent(Events::CollisionEnded* event);
		void OnLogicUpdateEvent(Events::LogicUpdate * event);

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
		ZilchDeclareDerivedType(FreezeEnabler, Component);
#endif

	private:
		// Member variables
	};

}