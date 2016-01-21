/*****************************************************************************/
/*!
@file   LockField.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "LockField.h"
#include "../../CoreComponents.h"

namespace DCEngine {
	namespace Components {

		/**************************************************************************/
		/*!
		@brief Provides the definition of this class to Zilch.
		@note This can only go in the translational unit (.cpp)
		*/
		/**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
		ZilchDefineType(LockField, "LockField", Rebound, builder, type) {
			DCE_BINDING_DEFINE_PROPERTY(LockField, Timer);
		}
#endif


		void LockField::Initialize()
		{
			auto gameObj = dynamic_cast<GameObject*>(Owner());
			TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Transform>(); // ew
			SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Sprite>();
		}

		void LockField::OnMouseDownEvent(Events::MouseDown * event)
		{

		}

		void LockField::OnCollisionStartedEvent(Events::CollisionStarted * event)
		{
			if (event->OtherObject->getComponent<Components::BallController>())
			{
				BallRef = event->OtherObject;
				BallRef->getComponent<Components::Transform>()->setTranslation(TransformRef->Translation);
				BallRef->getComponent<Components::BallController>()->Locked = true;
				BallRef->getComponent<Components::RigidBody>()->setDynamicState(DynamicStateType::Static);
			}
			if (event->OtherObject->getComponent<Components::PlayerController>() && BallRef)
			{
				BallRef->getComponent<Components::RigidBody>()->setDynamicState(DynamicStateType::Dynamic);
				BallRef->getComponent<Components::BallController>()->Locked = false;
				BallRef = NULL;
			}
		}

		void LockField::OnCollisionEndedEvent(Events::CollisionEnded * event)
		{
		}

		void LockField::OnMouseUpEvent(Events::MouseUp * event)
		{

		}

		void LockField::OnKeyDownEvent(Events::KeyDown * event)
		{
		}

		void LockField::OnKeyUpEvent(Events::KeyDown * event)
		{
		}

		void LockField::OnLogicUpdateEvent(Events::LogicUpdate * event)
		{

		}


	}
}