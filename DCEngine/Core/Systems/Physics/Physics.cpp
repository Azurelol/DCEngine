#include "Physics.h"
#include "../../Components/Transform.h"
#include "../../Components/RigidBody.h"
#include "../../Components/BoxCollider.h"
#include "../../Objects/Entities/EntitiesInclude.h"

namespace DCEngine {
	namespace Systems {

		/**************************************************************************/
		/*!
		\brief Constructor for the Physics system.
		*/
		/**************************************************************************/
		Physics::Physics() : System(std::string("PhysicsSystem"), EnumeratedSystem::Physics) {
		}

		/**************************************************************************/
		/*!
		\brief Initializes the Physics system.
		*/
		/**************************************************************************/
		void Physics::Initialize()
		{

		}

		/**************************************************************************/
		/*!
		\brief Registers a space to this system.
		*/
		/**************************************************************************/
		void Physics::RegisterSpace(PhysicsSpace & physicsSpace)
		{
			physicsSpaces_.push_back(&physicsSpace);
			trace << "Physics::Register -  " << physicsSpace.Owner()->Name()
				<< " has registered to the Physics system\n";
		}

		/**************************************************************************/
		/*!
		\brief The main update function for all physical bodies.
		*/
		/**************************************************************************/
		void Physics::Update(float dt) {
			// Iterate through every space that has the 'PhysicsSpace' component
			for (auto physpace : physicsSpaces_)
			{
				//GameObjectRawVec pairs = BroadPhaseDetection(physpace);
        //NarrowPhaseDetection(pairs);

				// For all gameobjects with a 'BoxCollider' component
				for (auto bc : physpace->getColliders())
				{
          
				}
			}
		}

		/**************************************************************************/
		/*!
		\brief Do
		*/
		/**************************************************************************/
		void Physics::Integrate()
		{
		}

		/**************************************************************************/
		/*!
		\brief set up pairs of objects that might be colliding
		*/
		/**************************************************************************/

		GameObjectRawVec Physics::BroadPhaseDetection(PhysicsSpace* physpace)
		{
			// For all gameobjects with a 'RigidBody' component
			auto list = physpace->getRigidbodies();

			GameObjectRawVec result;

			for (int i = 0; i < list.size(); ++i)
			{
				for (int j = i + 1; i < list.size(); ++j)
				{
					result.push_back(list[i]);
					result.push_back(list[j]);
				}
			}

			return result;
		}

		/**************************************************************************/
		/*!
		\brief actually detect if collision is happening and store info about it
		*/
		/**************************************************************************/
		Manifold Physics::NarrowPhaseDetection(GameObjectRawVec pairs)
		{
			GameObject * obj1, *obj2;
			
			//trace << _name << "::NarrowPhase \n";
			for (int i = 0; i < pairs.size(); ++i)
			{
				/* set pointers to the objects we are checking collision with */
				obj1 = pairs[i];
				obj2 = pairs[++i];

				if (BoxtoBox(obj1, obj2))
				{
					// generate collision data
					trace << "Colision\n";
				}


			}
			return Manifold();
		}

		bool Physics::BoxtoBox(GameObject * obj1, GameObject * obj2)
		{
			Component *rigidbody1, *rigidbody2, *boxcollider1, *boxcollider2, *transform1, *transform2;

			/* get the rigidbodies */
			rigidbody1 = obj1->getComponentByName(std::string("RigidBody"));
			rigidbody2 = obj2->getComponentByName(std::string("RigidBody"));

			/* get the colliders */
			boxcollider1 = obj1->getComponentByName(std::string("BoxCollider"));
			boxcollider2 = obj2->getComponentByName(std::string("BoxCollider"));

			/* get the transforms */
			transform1 = obj1->getComponentByName(std::string("Transform"));
			transform2 = obj2->getComponentByName(std::string("Transform"));





			return true;
		}

		bool Physics::CircletoBox(GameObject * obj1, GameObject * obj2)
		{
			return false;
		}

		bool Physics::CircletoCircle(GameObject * obj1, GameObject * obj2)
		{
			return false;
		}

		/**************************************************************************/
		/*!
		\brief resolve the collisions
		*/
		/**************************************************************************/
		void Physics::Resolve(Manifold data)
		{
		}


		/**************************************************************************/
		/*!
		\brief Terminate the physics system.
		*/
		/**************************************************************************/
		void Physics::Terminate() {
		}

	}

}

// 1. Integration: F->V->P (rigidbody)

// 2. Broadphase: Pair(collider) (collider)

// 3. Narrowphase: Input(Pairs), Output(Manifold) (collider)
// Whatever contact information is sent through

// Manifold: pair. normal. Points(2), PenetrationDistance
// Information to solve collisions in 2D.
// Ou can also have poitns in bodyspace[1]

// n-square bounding box. wrap in AABB

// 4. Resolution (Impulse solver): Input(Manifolds), (rigidbody)
/* for (0 -> 10) number of iterations
solve all manifolds

Rigidbody holds mass information
collider holds collision information

functions SphereSphere, BoxSphere, BoxBox
give each collider an ID )is it a box,  a sphere
Then do a collision lookup table. An array of function pointers.
lists for performance
n-list

virtual interface for the collider.

physis

*/

// 5. Send events to the objects who have collided after resolution.  