/*****************************************************************************/
/*!
@file   Skeleton.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Skeleton.h"
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
    ZilchDefineType(Skeleton, "Skeleton", Rebound, builder, type) {
		DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Skeleton);
      DCE_BINDING_DEFINE_PROPERTY(Skeleton, Timer);
    }
#endif


    void Skeleton::Initialize()
    {
      auto gameObj = dynamic_cast<GameObject*>(Owner());
      TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Transform>(); // ew
      SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Sprite>();
    }

    void Skeleton::OnMouseDownEvent(Events::MouseDown * event)
    {

    }

    void Skeleton::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
    }

    void Skeleton::OnCollisionEndedEvent(Events::CollisionEnded * event)
    {
    }

	void Skeleton::OnCollisionPersistedEvent(Events::CollisionPersisted * event)
	{
	}

    void Skeleton::OnMouseUpEvent(Events::MouseUp * event)
    {

    }

    void Skeleton::OnKeyDownEvent(Events::KeyDown * event)
    {
    }

    void Skeleton::OnKeyUpEvent(Events::KeyDown * event)
    {
    }

    void Skeleton::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {

    }


  }
}