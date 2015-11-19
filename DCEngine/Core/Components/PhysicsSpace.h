/*****************************************************************************/
/*!
\file   PhysicSpaces.h
\author Blaine Reiner, Christian Sagel
\par    email: blaine.reiner@digipen.edu, c.sagel\@digipen.edu
\date   9/23/2015
\brief  The physics space hold the settings for all physical interactions within
        a space. It also holds several containers of objects that require
        physics integration and resolution, for collisions, velocity, etc...
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"
#include "../Objects/Entities/GameObject.h"
#include "../Resources/CollisionGroup.h"
#include "../Resources/CollisionTable.h"

namespace DCEngine {

  class GameObject;

  class Physics;
  class PhysicsSpace : public Component {
    friend class Physics;
  public:

    // Properties
    void setCollisionTable(CollisionTableHandle);
    CollisionTableHandle getCollisionTable();

    // Methods
    void AddRigidBody(GameObject* rigidbody);
    void RemoveRigidBody(GameObject* rigidbody);
    void AddCollider(GameObject* collider);
    void RemoveCollider(GameObject* collider);
    GameObjectRawVec getRigidbodies();
    GameObjectRawVec getColliders();

    PhysicsSpace(Entity& owner);
    ~PhysicsSpace();
    void Initialize();


  private:

    bool AllowSleep;
    bool Mode2D;
    bool Deterministic;
    CollisionTableHandle CollisionTable;

    GameObjectRawVec RigidBodiesContainer;
    GameObjectRawVec CollidersContainer;

  };


}