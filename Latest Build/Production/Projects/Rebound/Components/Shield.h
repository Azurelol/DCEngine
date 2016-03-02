/*****************************************************************************/
/*!
@file   Shield.h
@author Jason Jorgenson
@par    email: j.jorgenson\@digipen.edu
@date   3-1-16
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
    class BoxCollider;
    class Shield : public Component {

    public:
      Transform* TransformRef;
      RigidBody* RigidBodyRef;
      Sprite* SpriteRef;
      BoxCollider* BoxColliderRef;
      CollisionTablePtr CollisionTable;

      // Properties
      DCE_DEFINE_PROPERTY(Real, Timer);

      // Methods
      Shield(Entity& owner) : Component(std::string("Shield"), owner) {}
      void Initialize();
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnMouseUpEvent(Events::MouseUp* event);
      void OnKeyDownEvent(Events::KeyDown* event);
      void OnKeyUpEvent(Events::KeyDown* event);
      void OnCollisionStartedEvent(Events::CollisionStarted* event);
      void OnCollisionEndedEvent(Events::CollisionEnded* event);
      void OnCollisionPersistedEvent(Events::CollisionPersisted * event);
      void OnLogicUpdateEvent(Events::LogicUpdate * event);

      void Deactivate();

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(Shield, Component);
#endif

    private:
      // Member variables
      Real Timer = 0;
    };
  }

}