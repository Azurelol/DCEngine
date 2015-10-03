#include "Physics.h"

#include "../../Components/Transform.h"
#include "../../Components/RigidBody.h"
#include "../../Components/BoxCollider.h"
#include "../../Objects/Entities/EntitiesInclude.h"
#include "../../Events/CollisionEvents.h"

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
		void Physics::Update(float dt)
		{
			Step(dt);
		}

		void Physics::Step(float dt)
		{

			// Iterate through every space that has the 'PhysicsSpace' component
			for (auto physpace : physicsSpaces_)
			{
				auto pairs = BroadPhaseDetection(physpace);

				NarrowPhaseDetection(pairs);

				Integrate(dt, physpace);

				//Contacts.Reset();

				//DetectContacts(dt);

				//Contacts.ResolveContacts(dt);

				PublishResults(physpace);
			}


		}

		/**************************************************************************/
		/*!
		\brief  go through all the rigid bodies and update velocity
		*/
		/**************************************************************************/
		void Physics::Integrate(float dt, PhysicsSpace* physpace)
		{
			auto bodies = physpace->getRigidbodies();

			for (int i = 0; i < bodies.size(); ++i)
			{
				bodies[i]->getComponent<RigidBody>()->Integrate(dt);
			}
		}

		/**************************************************************************/
		/*!
		\brief go through all the rigid bodies and update position
		*/
		/**************************************************************************/
		void Physics::PublishResults(PhysicsSpace* physpace)
		{
			auto bodies = physpace->getRigidbodies();

			for (int i = 0; i < bodies.size(); ++i)
			{
				bodies[i]->getComponent<RigidBody>()->PublishResults();
			}
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
				for (int j = i + 1; j < list.size(); ++j)
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

				// COLLISION DETECTED
				if (BoxtoBox(obj1, obj2))
				{
					// generate collision data

					// TEMPORARY: SEND EVENT DIRECTLY TO OBJECTS
					CollisionData boxToBoxCollision;
					boxToBoxCollision.Object = obj1;
					boxToBoxCollision.OtherObject = obj2;
					DispatchCollisionStarted(boxToBoxCollision);

					//trace << "Physics::NarrowPhaseDetection - Colision between " << obj1->Name() << " and " << obj2->Name() << "\n";
				}
				// NO COLLISION DETECTED
				else {
					CollisionData boxToBoxCollision;
					boxToBoxCollision.Object = obj1;
					boxToBoxCollision.OtherObject = obj2;
					DispatchCollisionEnded(boxToBoxCollision);
				}


			}
			return Manifold();
		}

		bool Physics::BoxtoBox(GameObject * obj1, GameObject * obj2)
		{
			/* get the rigidbodies */
			auto rigidbody1 = obj1->getComponent<RigidBody>();
			auto rigidbody2 = obj2->getComponent<RigidBody>();

			/* get the colliders */
			auto boxcollider1 = obj1->getComponent<BoxCollider>();
			auto boxcollider2 = obj2->getComponent<BoxCollider>();

			/* get the transforms */
			auto transform1 = obj1->getComponent<Transform>();
			auto transform2 = obj2->getComponent<Transform>();


			float topA = transform1->Translation.y + 0.5f * boxcollider1->getSize().y;
			float leftA = transform1->Translation.x - 0.5f * boxcollider1->getSize().x;
			float rightA = transform1->Translation.x + 0.5f * boxcollider1->getSize().x;
			float bottomA = transform1->Translation.y - 0.5f * boxcollider1->getSize().y;

			float topB = transform2->Translation.y + 0.5f * boxcollider2->getSize().y;
			float leftB = transform2->Translation.x - 0.5f * boxcollider2->getSize().x;
			float rightB = transform2->Translation.x + 0.5f * boxcollider2->getSize().x;
			float bottomB = transform2->Translation.y - 0.5f * boxcollider2->getSize().y;

			if (leftA > rightB)
			{
				return false;
			}

			if (leftB > rightA)
			{
				return false;
			}

			if (topA < bottomB)
			{
				return false;
			}

			if (topB < bottomA)
			{
				return false;
			}

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

		void Physics::DispatchCollisionStarted(CollisionData & collisionData)
		{
			auto collisionStartedEvent = new Events::CollisionStarted();
			// Dispatch collision event to the first object
			collisionStartedEvent->Object = collisionData.Object;
			collisionStartedEvent->OtherObject = collisionData.OtherObject;
			collisionData.Object->Dispatch<Events::CollisionStarted>(collisionStartedEvent);
			// Dispatch collision event to the second object
			collisionStartedEvent->Object = collisionData.OtherObject;
			collisionStartedEvent->OtherObject = collisionData.Object;
			collisionData.OtherObject->Dispatch<Events::CollisionStarted>(collisionStartedEvent);
		}

		void Physics::DispatchCollisionEnded(CollisionData & collisionData)
		{
			auto collisionEndedEvent = new Events::CollisionEnded();
			// Dispatch collision event to the first object
			collisionEndedEvent->Object = collisionData.Object;
			collisionEndedEvent->OtherObject = collisionData.OtherObject;
			collisionData.Object->Dispatch<Events::CollisionEnded>(collisionEndedEvent);
			// Dispatch collision event to the second object
			collisionEndedEvent->Object = collisionData.OtherObject;
			collisionEndedEvent->OtherObject = collisionData.Object;
			collisionData.OtherObject->Dispatch<Events::CollisionEnded>(collisionEndedEvent);
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