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

// PhysicsCasting
#include "../Systems/Physics/PhysicsCasting.h"
// Components
#include "../Components/BoxCollider.h"
#include "../Components/RigidBody.h"
// Resources
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
      
      // Physics Casting            
      CastResult CastRay(Ray& ray);
      CastResult CastRay(Ray& ray, CastFilter& filter);
      CastResultsRange CastRay(Ray& ray, unsigned count);
      CastResultsRange CastRay(Ray& ray, unsigned count, CastFilter& filter);
      CastResultsRange CastSegment(Vec3& start, Vec3& end, unsigned count);
      CastResultsRange CastSegment(Vec3& start, Vec3& end, unsigned count, CastFilter& filter);
      CastResultsRange CastAabb(Vec3& center, Vec3& size, unsigned count, CastFilter& filter);
      CastResultsRange CastSphere(Vec3& center, float radius, unsigned count, CastFilter& filter);
      CastResultsRange CastCollider(Vec3& offset, Collider& testCollider, CastFilter& filter);

      // Accessors      
      RigidBodyContainer& AllRigidBodies();
      ColliderContainer& AllColliders();

      // Add
      void AddRigidBody(RigidBody& rigidbody);
      void RemoveRigidBody(RigidBody& rigidbody);
      void AddCollider(Collider* collider);
      void RemoveCollider(Collider* collider);

      // Properties
      DCE_DEFINE_PROPERTY(CollisionTableHandle, CollisionTable);
      DCE_DEFINE_PROPERTY(bool, AllowSleep);
      DCE_DEFINE_PROPERTY(bool, Mode2D);
      DCE_DEFINE_PROPERTY(bool, Deterministic);
      DCE_DEFINE_PROPERTY(float, Gravity);

      ZilchDeclareDerivedType(PhysicsSpace, Component);
      PhysicsSpace(Entity& owner);
      ~PhysicsSpace();
      void Initialize();

      float MinX = 0;
      float MaxX = 0;
      float MinY = 0;
      float MaxY = 0;
      float Gravity = 45.0f;

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