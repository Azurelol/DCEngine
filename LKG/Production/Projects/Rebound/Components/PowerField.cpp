/*****************************************************************************/
/*!
@file   PowerField.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "PowerField.h"
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
		ZilchDefineType(PowerField, "PowerField", Rebound, builder, type) {
			DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(PowerField);
			DCE_BINDING_DEFINE_PROPERTY(PowerField, Target);
		}
#endif


		void PowerField::Initialize()
		{
			auto gameObj = dynamic_cast<GameObject*>(Owner());
			//Connect(Daisy->getMouse(), Events::MouseDown, PowerField::OnMouseDownEvent);
			//Connect(Daisy->getMouse(), Events::MouseUp, PowerField::OnMouseUpEvent);
			//Connect(SpaceRef, Events::LogicUpdate, PowerField::OnLogicUpdateEvent);
			Connect(gameObj, Events::CollisionStarted, PowerField::OnCollisionStartedEvent);
			TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Transform>(); // ew
			SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Sprite>();
		}

		void PowerField::OnMouseDownEvent(Events::MouseDown * event)
		{

		}

		void PowerField::OnCollisionStartedEvent(Events::CollisionStarted * event)
		{
			if (event->OtherObject->getComponent<Components::BallController>())
			{
				auto BallRef = event->OtherObject;
				BallRef->getComponent<Components::Transform>()->setTranslation(TransformRef->Translation);
				BallRef->getComponent<Components::BallController>()->Powering = true;
				BallRef->getComponent<Components::RigidBody>()->setDynamicState(DynamicStateType::Static);
				auto wall = SpaceRef->FindObjectByName(Target);
				if (wall)
				{
          if (wall->getComponent<Components::MoveToLocation>())
          {
            wall->getComponent<Components::MoveToLocation>()->setMoving(true);
          }
          else if (wall->getComponent<Components::Shield>())
          {
            wall->getComponent<Components::Shield>()->Deactivate();
          }
				}

			}
		}

		void PowerField::OnCollisionEndedEvent(Events::CollisionEnded * event)
		{
			if (event->OtherObject->getComponent<Components::BallController>())
			{
				auto wall = SpaceRef->FindObjectByName(Target);
				if (wall)
				{
					if (wall->getComponent<Components::MoveToLocation>()->getMoving())
					{
						auto temp = wall->getComponent<Components::MoveToLocation>()->getInitialLocation();
						wall->getComponent<Components::MoveToLocation>()->setInitialLocation(wall->getComponent<Components::MoveToLocation>()->getTargetLocation());
						wall->getComponent<Components::MoveToLocation>()->setTargetLocation(temp);
					}
					else
					{
						wall->getComponent<Components::MoveToLocation>()->setMoving(true);
					}
				}
			}
		}

		void PowerField::OnMouseUpEvent(Events::MouseUp * event)
		{

		}

		void PowerField::OnKeyDownEvent(Events::KeyDown * event)
		{
		}

		void PowerField::OnKeyUpEvent(Events::KeyDown * event)
		{
		}

		void PowerField::OnLogicUpdateEvent(Events::LogicUpdate * event)
		{

		}


	}
}