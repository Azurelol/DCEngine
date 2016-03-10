/*****************************************************************************/
/*!
@file   FreezeEnabler.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "FreezeEnabler.h"
#include "../../CoreComponents.h"

namespace DCEngine {
  namespace Components
  {
    /**************************************************************************/
/*!
@brief Provides the definition of this class to Zilch.
@note This can only go in the translational unit (.cpp)
*/
/**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(FreezeEnabler, "FreezeEnabler", DCEngineCore, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(FreezeEnabler);
      //// Constructor / Destructor
      //ZilchBindConstructor(builder, type, FreezeEnabler, "owner", Entity&);
      //ZilchBindDestructor(builder, type, FreezeEnabler);
      //// Properties
      //ZilchBindProperty(builder, type, &Transform::getTranslation, &Transform::setTranslation, "Translation");
      //ZilchBindProperty(builder, type, &Transform::getRotation, &Transform::setRotation, "Rotation");
      //ZilchBindProperty(builder, type, &Transform::getScale, &Transform::setScale, "Scale");
    }
#endif


    void FreezeEnabler::Initialize()
    {
      auto gameObj = dynamic_cast<GameObject*>(Owner());
      //Connect(Daisy->getMouse(), Events::MouseDown, FreezeEnabler::OnMouseDownEvent);
      //Connect(Daisy->getMouse(), Events::MouseUp, FreezeEnabler::OnMouseUpEvent);
      //Connect(SpaceRef, Events::LogicUpdate, FreezeEnabler::OnLogicUpdateEvent);
      Connect(gameObj, Events::CollisionStarted, FreezeEnabler::OnCollisionStartedEvent);
      TransformRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>(); // ew
      SpriteRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Sprite>();
    }

    void FreezeEnabler::OnMouseDownEvent(Events::MouseDown * event)
    {

    }

    void FreezeEnabler::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
      if (event->OtherObject->getComponent<Components::PlayerController>())
      {
        auto ball = SpaceRef->FindObjectByName("Basketball");
        if (ball)
        {
          ball->getComponent<Components::BallController>()->FreezeEnabled = true;
          TransformRef->setTranslation(Vec3(0, -100, 0));
        }
      }
    }

    void FreezeEnabler::OnCollisionEndedEvent(Events::CollisionEnded * event)
    {
    }

    void FreezeEnabler::OnMouseUpEvent(Events::MouseUp * event)
    {

    }

    void FreezeEnabler::OnKeyDownEvent(Events::KeyDown * event)
    {
    }

    void FreezeEnabler::OnKeyUpEvent(Events::KeyDown * event)
    {
    }

    void FreezeEnabler::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {

    }

  }

}