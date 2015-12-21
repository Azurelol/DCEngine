/*****************************************************************************/
/*!
@file   MoveToLocation.h
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"

namespace DCEngine {
  namespace Components
  {

    class Transform;
    class RigidBody;
    class Sprite;
    class MoveToLocation : public Component {

    public:
      Transform* TransformRef;
      //RigidBody* RigidBodyRef;
      //Sprite* SpriteRef;

      // Properties
      DCE_DEFINE_PROPERTY(Real, Timer);
      DCE_DEFINE_PROPERTY(Boolean, Moving);
      DCE_DEFINE_PROPERTY(Real, MoveToLocationTime); //How long in seconds it takes to MoveToLocation to TargetLocation
      DCE_DEFINE_PROPERTY(Vec3, InitialLocation);
      DCE_DEFINE_PROPERTY(Vec3, TargetLocation);

      // Methods
      MoveToLocation(Entity& owner) : Component(std::string("MoveToLocation"), owner) {}
      void Initialize();
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnMouseUpEvent(Events::MouseUp* event);
      void OnKeyDownEvent(Events::KeyDown* event);
      void OnKeyUpEvent(Events::KeyDown* event);
      void OnCollisionStartedEvent(Events::CollisionStarted* event);
      void OnCollisionEndedEvent(Events::CollisionEnded* event);
      void OnLogicUpdateEvent(Events::LogicUpdate * event);

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(MoveToLocation, Component);
#endif

    private:
      // Member variables
      Real Timer = 0;
      Vec3 TargetLocation = Vec3(10.0f, 0.0f, 0.0f);
      Vec3 TempTargetLocation = Vec3(10.0f, 0.0f, 0.0f);
      Vec3 InitialLocation = Vec3(-9001.0f, -9001.0f, -9001.0f);
      Boolean Moving = true;
      Real MoveToLocationTime = 1;
    };

  }
}