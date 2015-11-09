#include "Physics.h"

#include "../../Components/Transform.h"
#include "../../Components/RigidBody.h"
#include "../../Components/BoxCollider.h"
#include "../../Components/CircleCollider.h"
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
			DCTrace << "Physics::Register -  " << physicsSpace.Owner()->Name()
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
			Step(1.0f / 60.0f);
		}

    /**************************************************************************/
    /*!
    @brief Checks for collision between the target position and an object
           within the space. It will return the first object hit.
    @param The delta time.
    */
    /**************************************************************************/
    GameObjectPtr Physics::FindObjectAtPosition(Vec3 pos, Space & space)
    {
      return GameObjectPtr();
    }

    /**************************************************************************/
    /*!
    @brief Checks for any GameObjects within the target position in the space.
    @param pos The position in the space.
    @param space A reference to the space.
    @return A container of all objects within the position. 
    */
    /**************************************************************************/
    GameObjectVec Physics::FindAllObjectsAtPosition(Vec3 pos, Space & space)
    {
      GameObjectVec objsAtPos;

      // Check for objects in the space for a collision
      for (auto gameObj : *space.AllObjects()) {
        // If there's a match, add it to the container
        if (PointToRectangle(gameObj.get(), pos))
          objsAtPos.push_back(gameObj);
      }      

      return objsAtPos;
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

				if (bodies[i]->getComponent<RigidBody>() != NULL)
				{
					bodies[i]->getComponent<RigidBody>()->Integrate(dt);
				}
				else
				{
					throw DCException("An object without a RigidBody got into the list of RigidBodies");
				}
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
				if (bodies[i]->getComponent<RigidBody>() != NULL)
				{
					bodies[i]->getComponent<RigidBody>()->PublishResults();
				}
				else
				{
					throw DCException("An object without a RigidBody got into the list of RigidBodies");
				}
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
			auto list = physpace->getColliders();

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


				if (obj1->getComponent<BoxCollider>() && obj2->getComponent<BoxCollider>())
				{
					// COLLISION DETECTED
					if (BoxtoBox(obj1, obj2, collision))
					{

            if (obj1->getComponent<BoxCollider>()->getGhost() == false && obj2->getComponent<BoxCollider>()->getGhost() == false)
            {
              contactlist.push_back(collision);
            }
            
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
				else if (obj1->getComponent<CircleCollider>() && obj2->getComponent<CircleCollider>())
				{
					if (CircletoCircle(obj1, obj2, collision))
					{
            if (obj1->getComponent<CircleCollider>()->getGhost() == false && obj2->getComponent<CircleCollider>()->getGhost() == false)
            {
              contactlist.push_back(collision);
            }
						// TEMPORARY: SEND EVENT DIRECTLY TO OBJECTS
						CollisionData circleToCirlceCollision;
						circleToCirlceCollision.Object = obj1;
						circleToCirlceCollision.OtherObject = obj2;
						DispatchCollisionStarted(circleToCirlceCollision);

					}
					// NO COLLISION DETECTED
					else
					{
						CollisionData circleToCirlceCollision;
						circleToCirlceCollision.Object = obj1;
						circleToCirlceCollision.OtherObject = obj2;
						DispatchCollisionEnded(circleToCirlceCollision);
					}
				}
				else if ((obj1->getComponent<BoxCollider>() && obj2->getComponent<CircleCollider>()))
				{
					if (CircletoBox(obj1, obj2, collision))
					{
            if (obj1->getComponent<BoxCollider>()->getGhost() == false && obj2->getComponent<CircleCollider>()->getGhost() == false)
            {
              contactlist.push_back(collision);
            }
						// TEMPORARY: SEND EVENT DIRECTLY TO OBJECTS
						CollisionData boxToCirlceCollision;
						boxToCirlceCollision.Object = obj1;
						boxToCirlceCollision.OtherObject = obj2;
						DispatchCollisionStarted(boxToCirlceCollision);

					}
					// NO COLLISION DETECTED
					else
					{
						CollisionData boxToCirlceCollision;
						boxToCirlceCollision.Object = obj1;
						boxToCirlceCollision.OtherObject = obj2;
						DispatchCollisionEnded(boxToCirlceCollision);
					}
				}
				else if ((obj1->getComponent<CircleCollider>() && obj2->getComponent<BoxCollider>()))
				{
					if (CircletoBox(obj2, obj1, collision))
					{
            if (obj1->getComponent<CircleCollider>()->getGhost() == false && obj2->getComponent<BoxCollider>()->getGhost() == false)
            {
              contactlist.push_back(collision);
            }
						// TEMPORARY: SEND EVENT DIRECTLY TO OBJECTS
						CollisionData boxToCirlceCollision;
						boxToCirlceCollision.Object = obj2;
						boxToCirlceCollision.OtherObject = obj1;
						DispatchCollisionStarted(boxToCirlceCollision);

					}
					// NO COLLISION DETECTED
					else
					{
						CollisionData boxToCirlceCollision;
						boxToCirlceCollision.Object = obj2;
						boxToCirlceCollision.OtherObject = obj1;
						DispatchCollisionEnded(boxToCirlceCollision);
					}
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

	}

}
