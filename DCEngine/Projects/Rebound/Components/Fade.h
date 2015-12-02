/*****************************************************************************/
/*!
@file   Fade.h
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
	class Fade : public Component {

	public:
		Transform* TransformRef;
		RigidBody* RigidBodyRef;
		Sprite* SpriteRef;

		// Properties
		DCE_DEFINE_PROPERTY(Real, Timer);
		DCE_DEFINE_PROPERTY(Boolean, Fading);
		DCE_DEFINE_PROPERTY(Boolean, Pulse);
		DCE_DEFINE_PROPERTY(Real, FadeTime); //How long in seconds it takes to fade to TargetColor
		DCE_DEFINE_PROPERTY(Vec4, InitialColor);
		DCE_DEFINE_PROPERTY(Vec4, TargetColor);
		// Methods
		Fade(Entity& owner) : Component(std::string("Fade"), owner) {}
		void Initialize();
		void OnMouseDownEvent(Events::MouseDown* event);
		void OnMouseUpEvent(Events::MouseUp* event);
		void OnKeyDownEvent(Events::KeyDown* event);
		void OnKeyUpEvent(Events::KeyDown* event);
		void OnCollisionStartedEvent(Events::CollisionStarted* event);
		void OnCollisionEndedEvent(Events::CollisionEnded* event);
		void OnLogicUpdateEvent(Events::LogicUpdate * event);

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
		ZilchDeclareDerivedType(Fade, Component);
#endif

	private:
		// Member variables
		Real Timer = 0;
		Vec4 TargetColor = Vec4(1, 1, 1, 1);
		Vec4 InitialColor = Vec4(-1.0f, -1.0f, -1.0f, -1.0f);
		Boolean Fading = false;
		Boolean Pulse = true;
		Real FadeTime = 1;
	};

}