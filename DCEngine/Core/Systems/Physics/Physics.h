/******************************************************************************/
/*!
\file   Physics.h
\author Blaine Reiner
\par    email: blaine.reiner\@digipen.edu
\par    DigiPen login: blaine.reiner
\date   11/20/2015
\brief  Interface file of the physics system
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "../System.h"

// GameObject
#include "../../Objects/Entities/GameObject.h"
// Components
#include "../../Components/PhysicsSpace.h"
// Resources
#include "../../Resources/CollisionTable.h"
// Packages
#include "Manifold.h"
#include "Collision.h"
// PhysicsCasting
#include "PhysicsCasting.h"

namespace DCEngine {

  // Forward declarations
  class Engine;
  class Transform;
  namespace Components {
    class Collider;
  }

  struct CollisionData {
    GameObject* Object; //!< The object this event was sent to.
    GameObject* OtherObject; //!< The other object in the collision.
    CollisionFilter filter; //!< info on what events to send
    Vec3 Normal;
  };

  namespace PHYSICS
  {
    const float Gravity = 50.0f;
  }

  namespace Systems
  {
    struct DetectionPairing
    {
      GameObjectPtr obj1 = NULL;
      GameObjectPtr obj2 = NULL;
      CollisionFilter filter;
    };

    class Physics : public System
    {
      friend class Engine;
    public:
      bool IsObjectWithinBoundingArea(Vec3& center, float width, float height, GameObjectPtr gameobject);
      GameObjectPtr FindObjectAtPosition(Vec3 pos, Space& space);
      GameObjectVec FindAllObjectsAtPosition(Vec3 pos, Space& space);
      void RegisterSpace(Components::PhysicsSpace& physicsSpace);
      void DeregisterSpace(Components::PhysicsSpace& physicsSpace);

      // Physics Casting            
      CastResult CastRay(Ray& ray, Components::PhysicsSpace *Space);
      CastResult CastRay(Ray& ray, CastFilter& filter, Components::PhysicsSpace *Space);
      CastResultsRange CastRay(Ray& ray, unsigned count, Components::PhysicsSpace *Space);
      CastResultsRange CastRay(Ray& ray, unsigned count, CastFilter& filter, Components::PhysicsSpace *Space);
      CastResultsRange CastSegment(Vec3& start, Vec3& end, unsigned count, Components::PhysicsSpace *Space);
      CastResultsRange CastSegment(Vec3& start, Vec3& end, unsigned count, CastFilter& filter, Components::PhysicsSpace *Space);
      CastResultsRange CastAabb(Vec3& center, Vec3& size, unsigned count, CastFilter& filter, Components::PhysicsSpace *Space);
      CastResultsRange CastSphere(Vec3& center, float radius, unsigned count, CastFilter& filter, Components::PhysicsSpace *Space);
      CastResultsRange CastCollider(Vec3& offset, Components::Collider& testCollider, CastFilter& filter, Components::PhysicsSpace *Space);
      

    private:

      bool Paused = false;
      unsigned QuadTreeBucketSize = 6;
      std::vector<Components::PhysicsSpace*> physicsSpaces_;
      std::vector<std::pair<GameObjectPtr, GameObjectPtr>> PersistedPairs;
      std::vector<CollisionData> OnCollisionStarteds;
      std::vector<CollisionData> OnCollisionPersisteds;
      std::vector<CollisionData> OnCollisionEndeds;



      Physics();
      void Initialize();
      void Subscribe();
      void OnEnginePauseEvent(Events::EnginePause* event);
      void OnEngineResumeEvent(Events::EngineResume* event);
      void Update(float dt);
      void Terminate();

      bool Persisted(std::pair<GameObjectPtr, GameObjectPtr> &pair);
      void RemovePair(std::pair<GameObjectPtr, GameObjectPtr> &pair);
      void Integrate(float dt, Components::PhysicsSpace* physpace);
      void PublishResults(Components::PhysicsSpace* physpace);
      void UpdateTransforms(Components::PhysicsSpace *physpace);
      void Step(float dt);
      void BroadPhaseDetection(Components::PhysicsSpace* physpace , std::vector<DetectionPairing> &pairs);
      void NarrowPhaseDetection(std::vector<DetectionPairing> &pairs, std::vector<Manifold> &contactlist);
      void DispatchCollisionStarted(CollisionData& collisionData);
      void DispatchCollisionEnded(CollisionData& collisionData);
      void DispatchCollisionPersisted(CollisionData& collisionData);
      void SendEvents(void);

      float minX, maxX, minY, maxY;

    };

  }
}