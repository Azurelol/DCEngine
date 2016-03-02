/*****************************************************************************/
/*!
@file   TutorialTextLogic.h
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"

namespace DCEngine {
  namespace Components {
    class Transform;
    class RigidBody;
    class Sprite;
    class TutorialTextLogic : public Component {

    public:
      Transform* TransformRef;
      RigidBody* RigidBodyRef;
      Sprite* SpriteRef;

      // Properties
      DCE_DEFINE_PROPERTY(Real, Timer);
      DCE_DEFINE_PROPERTY(Real, Type);

      // Methods
      TutorialTextLogic(Entity& owner) : Component(std::string("TutorialTextLogic"), owner) {}
      void Initialize();
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnMouseUpEvent(Events::MouseUp* event);
      void OnKeyDownEvent(Events::KeyDown* event);
      void OnKeyUpEvent(Events::KeyDown* event);
      void OnCollisionStartedEvent(Events::CollisionStarted* event);
      void OnCollisionEndedEvent(Events::CollisionEnded* event);
      void OnLogicUpdateEvent(Events::LogicUpdate * event);

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(TutorialTextLogic, Component);
#endif

    private:
      // Member variables
      Real Timer = 0;
      Real Type = 0; //replace this with an enum once we can serialize those
    };
  }

}