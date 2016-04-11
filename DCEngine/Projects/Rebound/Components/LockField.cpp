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
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(LockField);
			DCE_BINDING_DEFINE_PROPERTY(LockField, Timer);
		}
#endif


		void LockField::Initialize()
		{
			auto gameObj = dynamic_cast<GameObject*>(Owner());
			Connect(gameObj, Events::CollisionStarted, LockField::OnCollisionStartedEvent);
			Connect(gameObj, Events::CollisionPersisted, LockField::OnCollisionPersistedEvent);
			Connect(gameObj, Events::CollisionEnded, LockField::OnCollisionEndedEvent);
			TransformRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>(); // ew
			SpriteRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Sprite>();
			SpriteRef->SpriteSource = "tile_collector_off";
			SpriteRef->Color = Vec4(1, 1, 1, 1);
		}

		void LockField::OnMouseDownEvent(Events::MouseDown * event)
		{

		}

		void LockField::OnCollisionStartedEvent(Events::CollisionStarted * event)
		{
			if (event->OtherObject->getComponent<Components::BallController>() != nullptr)
			{
				if (event->OtherObject->Parent() != nullptr)
				{
					return;
				}
				BallRef = event->OtherObject;
				LockBall(BallRef);
			}

			if (event->OtherObject->getComponent<Components::PlayerController>() != nullptr && BallRef)
			{
				UnlockBall(BallRef);
				BallRef = NULL;
			}
		}

		void LockField::LockBall(GameObject* ball)
		{
			ball->getComponent<Components::Transform>()->setTranslation(TransformRef->Translation);
			ball->getComponent<Components::BallController>()->Locked = true;
			SpriteRef->SpriteSource = "tile_collector_on";
			ball->getComponent<Components::RigidBody>()->setDynamicState(DynamicStateType::Static);
		}

		void LockField::UnlockBall(GameObject* ball)
		{
			ball->getComponent<Components::RigidBody>()->setDynamicState(DynamicStateType::Dynamic);
			ball->getComponent<Components::BallController>()->Locked = false;
			SpriteRef->SpriteSource = "tile_collector_off";
			ball->getComponent<Components::BallController>()->ParentToPlayer();
		}

		void LockField::OnCollisionEndedEvent(Events::CollisionEnded * event)
		{
		}

		void LockField::OnCollisionPersistedEvent(Events::CollisionPersisted * event)
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