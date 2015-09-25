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

	namespace Systems {
		class Physics : public System {
			friend class Engine;
		public:
			void RegisterSpace(PhysicsSpace& physicsSpace);

		private:

			std::vector<PhysicsSpace*> physicsSpaces_;

			Physics();
			void Initialize();
			void Update(float dt);
			void Terminate();

			void Integrate();
			GameObjectRawVec Physics::BroadPhaseDetection(PhysicsSpace* physpace);
			Manifold NarrowPhaseDetection(GameObjectRawVec pairs);
			bool BoxtoBox(GameObject *obj1, GameObject *obj2);
			bool CircletoBox(GameObject *obj1, GameObject *obj2);
			bool CircletoCircle(GameObject *obj1, GameObject *obj2);
			void Resolve(Manifold data);

		};

	}
}