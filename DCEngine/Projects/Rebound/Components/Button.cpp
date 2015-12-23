/*****************************************************************************/
/*!
@file   Button.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Button.h"
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
    ZilchDefineType(Button, "Button", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Button);
      DCE_BINDING_DEFINE_PROPERTY(Button, Target);
    }
#endif


    void Button::Initialize()
    {
      auto gameObj = dynamic_cast<GameObject*>(Owner());
      //Connect(Daisy->getMouse(), Events::MouseDown, Button::OnMouseDownEvent);
      //Connect(Daisy->getMouse(), Events::MouseUp, Button::OnMouseUpEvent);
      //Connect(SpaceRef, Events::LogicUpdate, Button::OnLogicUpdateEvent);
      Connect(gameObj, Events::CollisionStarted, Button::OnCollisionStartedEvent);
      TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Transform>(); // ew
      SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Sprite>();
    }

    void Button::OnMouseDownEvent(Events::MouseDown * event)
    {

    }

    void Button::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
      if (event->OtherObject->getComponent<Components::BallController>())
      {
        auto ball = SpaceRef->FindObjectByName("Basketball");
        if (ball)
        {
          auto wall = SpaceRef->FindObjectByName("Target");
          if (wall)
          {
            wall->getComponent<Components::MoveToLocation>()->setMoving(true);
          }
        }
      }
    }

    void Button::OnCollisionEndedEvent(Events::CollisionEnded * event)
    {
    }

    void Button::OnMouseUpEvent(Events::MouseUp * event)
    {

    }

    void Button::OnKeyDownEvent(Events::KeyDown * event)
    {
    }

    void Button::OnKeyUpEvent(Events::KeyDown * event)
    {
    }

    void Button::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {

    }


  }
}