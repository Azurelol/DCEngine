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

	namespace Systems 
	{
		class Physics : public System 
		{
			friend class Engine;
		public:
			void RegisterSpace(PhysicsSpace& physicsSpace);

		private:

			std::vector<PhysicsSpace*> physicsSpaces_;

			Physics();
			void Initialize();
			void Update(float dt);
			void Terminate();

			void Integrate(float dt, PhysicsSpace* physpace);
			void PublishResults(PhysicsSpace* physpace);
			void Step(float dt);
			GameObjectRawVec BroadPhaseDetection(PhysicsSpace* physpace);
			Manifold NarrowPhaseDetection(GameObjectRawVec pairs);
			void Resolve(Manifold data);
      void DispatchCollisionStarted(CollisionData& collisionData);
      void DispatchCollisionEnded(CollisionData& collisionData);

		};

	}
}