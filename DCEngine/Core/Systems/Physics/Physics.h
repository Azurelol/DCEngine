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
#include "Raycasting.h"

namespace DCEngine {

  // Forward declarations
  class Engine;
  namespace Components {
    class Collider;
  }

  struct CollisionData {
    GameObject* Object; //!< The object this event was sent to.
    GameObject* OtherObject; //!< The other object in the collision.
    CollisionFilter filter; //!< info on what events to send
  };

  namespace PHYSICS
  {
    const float Gravity = 60.0f;
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
      GameObjectPtr FindObjectAtPosition(Vec3 pos, Space& space);
      GameObjectVec FindAllObjectsAtPosition(Vec3 pos, Space& space);
      void RegisterSpace(Components::PhysicsSpace& physicsSpace);
      void DeregisterSpace(Components::PhysicsSpace& physicsSpace);

      // Physics Casting            
      CastResult CastRay(Ray& ray);
      CastResult CastRay(Ray& ray, CastFilter& filter);
      CastResultsRange CastRay(Ray& ray, unsigned count);
      CastResultsRange CastRay(Ray& ray, unsigned count, CastFilter& filter);
      CastResultsRange CastSegment(Vec3& start, Vec3& end, unsigned count);
      CastResultsRange CastSegment(Vec3& start, Vec3& end, unsigned count, CastFilter& filter);
      CastResultsRange CastAabb(Vec3& center, Vec3& size, unsigned count, CastFilter& filter);
      CastResultsRange CastSphere(Vec3& center, float radius, unsigned count, CastFilter& filter);
      CastResultsRange CastCollider(Vec3& offset, Components::Collider& testCollider, CastFilter& filter);


    private:

      bool Paused = false;
      std::vector<Components::PhysicsSpace*> physicsSpaces_;
      std::vector<std::pair<GameObjectPtr, GameObjectPtr>> PersistedPairs;

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

    };

  }
}