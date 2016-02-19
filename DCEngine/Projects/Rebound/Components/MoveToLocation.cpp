/*****************************************************************************/
/*!
@file   MoveToLocation.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "MoveToLocation.h"
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
    ZilchDefineType(MoveToLocation, "MoveToLocation", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(MoveToLocation);
      DCE_BINDING_DEFINE_PROPERTY(MoveToLocation, Timer);
      DCE_BINDING_DEFINE_PROPERTY(MoveToLocation, TargetLocation);
      DCE_BINDING_DEFINE_PROPERTY(MoveToLocation, MoveToLocationTime);
      DCE_BINDING_DEFINE_PROPERTY(MoveToLocation, Moving);
    }

    void MoveToLocation::Initialize()
    {
      auto gameObj = dynamic_cast<GameObject*>(Owner());
      //Connect(Daisy->getKeyboard(), Events::KeyDown, MoveToLocation::OnKeyDownEvent);
      //Connect(Daisy->getKeyboard(), Events::KeyUp, MoveToLocation::OnKeyUpEvent);
      //Connect(Daisy->getMouse(), Events::MouseDown, MoveToLocation::OnMouseDownEvent);
      //Connect(Daisy->getMouse(), Events::MouseUp, MoveToLocation::OnMouseUpEvent);
      //Connect(gameObj, Events::CollisionStarted, MoveToLocation::OnCollisionStartedEvent);
      //Connect(gameObj, Events::CollisionEnded, MoveToLocation::OnCollisionEndedEvent);
      Connect(SpaceRef, Events::LogicUpdate, MoveToLocation::OnLogicUpdateEvent);
      //SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Components::Sprite>();
      TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Transform>();
      InitialLocation = TransformRef->getTranslation();
      TargetLocation = InitialLocation + TargetLocation;
    }

    void MoveToLocation::OnMouseDownEvent(Events::MouseDown * event)
    {

    }

    void MoveToLocation::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
    }

    void MoveToLocation::OnCollisionEndedEvent(Events::CollisionEnded * event)
    {
    }

    void MoveToLocation::OnMouseUpEvent(Events::MouseUp * event)
    {

    }

    void MoveToLocation::OnKeyDownEvent(Events::KeyDown * event)
    {
    }

    void MoveToLocation::OnKeyUpEvent(Events::KeyDown * event)
    {
    }

    void MoveToLocation::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
      if (!Moving)
        return;

      if (InitialLocation == Vec3(-9001.0f, -9001.0f, -9001.0f))
      {
        InitialLocation = TransformRef->getTranslation();
      }
      Timer += event->Dt;
      if (Timer > MoveToLocationTime)
      {
        Timer = MoveToLocationTime;
      }

      auto percentDone = Timer / MoveToLocationTime;
      //SpriteRef->Color = Interpolate(InitialColor, TargetColor, Timer / MoveToLocationTime);

      TransformRef->setTranslation(InitialLocation * (1 - percentDone) + TargetLocation * percentDone);
      if (TransformRef->getTranslation() == TargetLocation)
      {
        Moving = false;
        Timer = 0;
        TargetLocation = InitialLocation;
        InitialLocation = TransformRef->getTranslation();
      }
    }

  }

}