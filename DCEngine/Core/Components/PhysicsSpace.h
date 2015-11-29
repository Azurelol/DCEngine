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

  using RigidBodyContainer = std::vector<RigidBody*>;
  using ColliderContainer = std::vector<Collider*>;

  class Physics;
  class PhysicsSpace : public Component {
    friend class Physics;
  public:

    // PropertiesColli
    DCE_DEFINE_PROPERTY(CollisionTable, Table);
    void setCollisionTable(CollisionTable &table);
    CollisionTable *getCollisionTable(void);

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

    bool AllowSleep;
    bool Mode2D;
    bool Deterministic;

    CollisionTable Table;
    RigidBodyContainer RigidBodiesContainer;
    ColliderContainer CollidersContainer;

  };


}