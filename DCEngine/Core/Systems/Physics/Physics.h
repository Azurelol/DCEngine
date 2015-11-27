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
#include "../../Components/PhysicsSpace.h"
#include "Manifold.h"
#include "../../Resources/CollisionTable.h"
#include "../../Objects/Entities/GameObject.h"

namespace DCEngine {
  class Engine;

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
      GameObjectPtr obj1;
      GameObjectPtr obj2;
      CollisionFilter filter;
    };

    class Physics : public System
    {
      friend class Engine;
    public:
      GameObjectPtr FindObjectAtPosition(Vec3 pos, Space& space);
      GameObjectVec FindAllObjectsAtPosition(Vec3 pos, Space& space);
      void RegisterSpace(PhysicsSpace& physicsSpace);
      void DeregisterSpace(PhysicsSpace& physicsSpace);

    private:

      bool Paused;
      std::vector<PhysicsSpace*> physicsSpaces_;

      Physics();
      void Initialize();
      void Subscribe();
      void OnEnginePauseEvent(Events::EnginePause* event);
      void OnEngineResumeEvent(Events::EngineResume* event);
      void Update(float dt);
      void Terminate();

      void Integrate(float dt, PhysicsSpace* physpace);
      void PublishResults(PhysicsSpace* physpace);
      void UpdateTransforms(PhysicsSpace *physpace);
      void Step(float dt);
      std::vector<DetectionPairing> BroadPhaseDetection(PhysicsSpace* physpace);
      void NarrowPhaseDetection(std::vector<DetectionPairing> pairs, std::vector<Manifold> &contactlist);
      void Resolve(Manifold data);
      void DispatchCollisionStarted(CollisionData& collisionData);
      void DispatchCollisionEnded(CollisionData& collisionData);

    };

  }
}