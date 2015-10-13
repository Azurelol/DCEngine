#include "Physics.h"

#include "../../Components/Transform.h"
#include "../../Components/RigidBody.h"
#include "../../Components/BoxCollider.h"
#include "../../Objects/Entities/EntitiesInclude.h"
#include "../../Events/CollisionEvents.h"
#include "Collision.h"
#include "Resolution.h"

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
		@brief The main update function for all physical bodies.
    @param The delta time.
		*/
		/**************************************************************************/
		void Physics::Update(float dt)
		{
			Step(dt);
		}


		/**************************************************************************/
		/*!
		@brief The function that actually goes through the spaces and updates objects
    @param The delta time.
    @note  For every space, we update the objects in the following manner:
           1. 
		*/
		/**************************************************************************/
		void Physics::Step(float dt)
		{
			// Iterate through every space that has the 'PhysicsSpace' component
			for (auto physpace : physicsSpaces_)
			{
        std::vector<Manifold> contactlist;

				auto pairs = BroadPhaseDetection(physpace);

				NarrowPhaseDetection(pairs, contactlist);

				Integrate(dt, physpace);

        DCEngine::Resolve(dt, contactlist);

				//Contacts.Reset();

				//DetectContacts(dt);

				//Contacts.ResolveContacts(dt);

				PublishResults(physpace);
			}


		}

		/**************************************************************************/
		/*!
		@brief  Iterate through all the rigid bodies and update velocity.
    @param  The delta time.
    @param A pointer to the 'PhysicsSpace' component.
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
	  @brief Iterate through all the objects with a 'RigidBody' component and 
            update their position.
    @param A pointer to the 'PhysicsSpace' component.
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
		@brief Sets up pairs of objects that might be colliding with one another.
    @param A pointer to the 'PhysicsSpace' component.
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
		@brief Detect if collision is happening and store information about it.
    @param A vector of GameObjects.
		*/
		/**************************************************************************/
		void Physics::NarrowPhaseDetection(GameObjectRawVec pairs, std::vector<Manifold> &contactlist)
		{
			GameObject * obj1, *obj2;

      Manifold collision;

			for (int i = 0; i < pairs.size(); ++i)
			{
				/* set pointers to the objects we are checking collision with */
				obj1 = pairs[i];
				obj2 = pairs[++i];

				// COLLISION DETECTED
				if (BoxtoBox(obj1, obj2, collision))
				{
          contactlist.push_back(collision);
          // TEMPORARY: SEND EVENT DIRECTLY TO OBJECTS
					CollisionData boxToBoxCollision;
					boxToBoxCollision.Object = obj1;
					boxToBoxCollision.OtherObject = obj2;
					DispatchCollisionStarted(boxToBoxCollision);

				}
				// NO COLLISION DETECTED
				else {
					CollisionData boxToBoxCollision;
					boxToBoxCollision.Object = obj1;
					boxToBoxCollision.OtherObject = obj2;
					DispatchCollisionEnded(boxToBoxCollision);
				}


			}
			return;
		}



		/**************************************************************************/
		/*!
		@brief Resolve all collisions
    @param A manifold.
		*/
		/**************************************************************************/
		void Physics::Resolve(Manifold data)
		{
		}

    /**************************************************************************/
    /*!
    @brief Dispatch the event that signals that a collision has started between
           an object and another object.
    @param A struct that contains the collision data detected by the engine.
    */
    /**************************************************************************/
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

    /**************************************************************************/
    /*!
    @brief Dispatch the event that signals that a collision has ended between
           an object and another object.
    @param A struct that contains the collision data detected by the engine.
    */
    /**************************************************************************/
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

    void Physics::Serialize(Json::Value & root)
    {
    }

    void Physics::Deserialize(Json::Value & root)
    {
    }

	}

}
