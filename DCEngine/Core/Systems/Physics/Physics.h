/*****************************************************************************/
/*!
\file   Physics.h
\author Blaine Reiner, Christian Sagel
\par    email: blaine.reiner@digipen.edu, c.sagel\@digipen.edu
\date   9/23/2015
\brief  The physics system...
*/
/******************************************************************************/
#pragma once
#include "../System.h"
#include "../../Components/PhysicsSpace.h"
#include "Manifold.h"

namespace DCEngine {
  class Engine;

  struct CollisionData {
    GameObject* Object; //!< The object this event was sent to.
    GameObject* OtherObject; //!< The other object in the collision.
  };

  namespace PHYSICS
  {
    const float Gravity = 40.0f;
  }

  namespace Systems
  {
    class Physics : public System
    {
      friend class Engine;
    public:
      GameObjectPtr FindObjectAtPosition(Vec3 pos, Space& space);
      GameObjectVec FindAllObjectsAtPosition(Vec3 pos, Space& space);
      void RegisterSpace(PhysicsSpace& physicsSpace);

    private:

      std::vector<PhysicsSpace*> physicsSpaces_;

      Physics();
      void Initialize();
      void Update(float dt);
      void Terminate();

      void Integrate(float dt, PhysicsSpace* physpace);
      void PublishResults(PhysicsSpace* physpace);
      void UpdateTransforms(PhysicsSpace *physpace);
      void Step(float dt);
      GameObjectRawVec BroadPhaseDetection(PhysicsSpace* physpace);
      void NarrowPhaseDetection(GameObjectRawVec pairs, std::vector<Manifold> &contactlist);
      void Resolve(Manifold data);
      void DispatchCollisionStarted(CollisionData& collisionData);
      void DispatchCollisionEnded(CollisionData& collisionData);

    };

  }
}