/*****************************************************************************/
/*!
@file   Bomb.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Bomb.h"
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
		ZilchDefineType(Bomb, "Bomb", Rebound, builder, type) {
			DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Bomb);
			DCE_BINDING_DEFINE_PROPERTY(Bomb, Timer);
		}
#endif


		void Bomb::Initialize()
		{
			auto gameObj = dynamic_cast<GameObject*>(Owner());
			TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Transform>(); // ew
			SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Sprite>();
			Connect(SpaceRef, Events::LogicUpdate, Bomb::OnLogicUpdateEvent);
		}

		void Bomb::OnLogicUpdateEvent(Events::LogicUpdate * event)
		{

		}

		void Bomb::OnMouseDownEvent(Events::MouseDown * event)
		{

		}

		void Bomb::OnCollisionStartedEvent(Events::CollisionStarted * event)
		{

			if (event->OtherObject->getComponent<Components::BallController>())
			{
				auto gameObj = dynamic_cast<GameObject*>(Owner());
				gameObj->AttachTo(event->OtherObject);

			}
		}

		void Bomb::OnCollisionEndedEvent(Events::CollisionEnded * event)
		{
		}

		void Bomb::OnCollisionPersistedEvent(Events::CollisionPersisted * event)
		{
		}

		void Bomb::OnMouseUpEvent(Events::MouseUp * event)
		{

		}

		void Bomb::OnKeyDownEvent(Events::KeyDown * event)
		{
		}

		void Bomb::OnKeyUpEvent(Events::KeyDown * event)
		{
		}


	}
}