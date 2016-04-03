/*****************************************************************************/
/*!
@file   LancerShield.h
@author Jason Jorgenson
@par    email: j.jorgenson\@digipen.edu
@date   3/25/2016
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
    class LancerShield : public Component {

    public:
      Transform* TransformRef;
      RigidBody* RigidBodyRef;
      Sprite* SpriteRef;
      CollisionTablePtr CollisionTableRef;
      float ShieldReflectionForce;
      bool isActive;

      DCE_COMPONENT_DECLARE_DEPENDENCIES;

      // Properties
      DCE_DEFINE_PROPERTY(float, ShieldReflectionForce);

      // Methods
      LancerShield(Entity& owner) : Component(std::string("LancerShield"), owner) {}
      void Initialize();
      void OnCollisionStartedEvent(Events::CollisionStarted* event);
      void OnLogicUpdateEvent(Events::LogicUpdate * event);

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(LancerShield, Component);
#endif

    private:
      // Member variables
      GameObject* parent;
      
    };
  }

}