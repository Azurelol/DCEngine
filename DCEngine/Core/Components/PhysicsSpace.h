/*****************************************************************************/
/*!
\file   PhysicSpaces.h
\author Blaine Reiner, Christian Sagel
\par    email: blaine.reiner@digipen.edu, c.sagel\@digipen.edu
\date   9/23/2015
\brief  The physics space hold the settings for all physical interactions within
        a space. It also holds several containers of objects that require
        physics integration and resolution, for collisions, velocity, etc...
\todo   make get/set CollisionTable work with zilch/editor
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

#include "../Components/BoxCollider.h"
#include "../Components/RigidBody.h"
//#include "../Objects/Entities/GameObject.h"
#include "../Resources/CollisionGroup.h"
#include "../Resources/CollisionTable.h"

namespace DCEngine {

  namespace Components
  {
    using RigidBodyContainer = std::vector<RigidBody*>;
    using ColliderContainer = std::vector<Collider*>;

    class Physics;
    class PhysicsSpace : public Component {
      friend class Physics;
    public:

      #if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(PhysicsSpace, Component);
      #endif

      DCE_DEFINE_PROPERTY(CollisionTableHandle, CollisionTable);
      DCE_DEFINE_PROPERTY(bool, AllowSleep);
      DCE_DEFINE_PROPERTY(bool, Mode2D);
      DCE_DEFINE_PROPERTY(bool, Deterministic);
            
      // Methods
      void AddRigidBody(RigidBody& rigidbody);
      void RemoveRigidBody(RigidBody& rigidbody);
      void AddCollider(Collider* collider);
      void RemoveCollider(Collider* collider);

      RigidBodyContainer& AllRigidBodies();
      ColliderContainer& AllColliders();

      PhysicsSpace(Entity& owner);
      ~PhysicsSpace();
      void Initialize();

    private:

      bool AllowSleep = false;
      bool Mode2D = false;
      bool Deterministic = true;
      CollisionTableHandle CollisionTable;

      RigidBodyContainer RigidBodiesContainer;
      ColliderContainer CollidersContainer;

    };

  }

}