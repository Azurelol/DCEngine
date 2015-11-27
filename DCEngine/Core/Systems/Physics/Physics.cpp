/******************************************************************************/
/*!
\file   Physics.cpp
\author Blaine Reiner
\par    email: blaine.reiner\@digipen.edu
\par    DigiPen login: blaine.reiner
\date   11/20/2015
\brief  Implementaion of the physics system
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Physics.h"

// Engine
#include "../../Engine/Engine.h"
// Components 
#include "../../Components/Transform.h"
#include "../../Components/RigidBody.h"
#include "../../Components/BoxCollider.h"
#include "../../Components/CircleCollider.h"
#include "../../Objects/Entities/EntitiesInclude.h"
#include "../../Events/CollisionEvents.h"
// Custom physics libraries
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
      // Subscribe to events
      Subscribe();
		}

    /**************************************************************************/
    /*!
    \brief Subscribs the Physics system to events.
    */
    /**************************************************************************/
    void Physics::Subscribe()
    {
      Daisy->Connect<Events::EnginePause>(&Physics::OnEnginePauseEvent, this);
      Daisy->Connect<Events::EngineResume>(&Physics::OnEngineResumeEvent, this);
    }

    /**************************************************************************/
    /*!
    @brief Pauses the physics updates.
    @param event A pointer to the event.
    */
    /**************************************************************************/
    void Physics::OnEnginePauseEvent(Events::EnginePause * event)
    {
      DCTrace << "Physics::OnEnginePausedEvent - Pause \n";
      this->Paused = true;
    }

    /**************************************************************************/
    /*!
    @brief Resumes the physics updates.
    @param event A pointer to the event.
    */
    /**************************************************************************/
    void Physics::OnEngineResumeEvent(Events::EngineResume * event)
    {
      DCTrace << "Physics::OnEngineResumeEvent - Resume \n";
      this->Paused = false;
    }

		/**************************************************************************/
		/*!
		@brief Registers a space to this system.
    @param physicsSpace A reference to the PhysicsSpace.
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
    @brief Deregisters a space to this system.
    @param physicsSpace A reference to the PhysicsSpace.
    */
    /**************************************************************************/
    void Physics::DeregisterSpace(PhysicsSpace & physicsSpace)
    {
      DCTrace << "Physics::Deregister -  " << physicsSpace.Owner()->Name()
              << " has deregistered from the Physics system\n";
      auto physicsSpacePtr = &physicsSpace;
      physicsSpaces_.erase(std::remove(physicsSpaces_.begin(),
                                       physicsSpaces_.end(), physicsSpacePtr),
                                       physicsSpaces_.end());
    }

		/**************************************************************************/
		/*!
		@brief The main update function for all physical bodies.
		@param The delta time.
		*/
		/**************************************************************************/
		void Physics::Update(float dt)
		{
      // Do not update physics while the engine is paused.
      if (Paused)
        return;

        Step(dt);
      
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
        if (Collision::PointToRectangle(gameObj, pos))
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

				Resolution::Resolve(dt, contactlist);

				PublishResults(physpace);

        UpdateTransforms(physpace);
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
			auto& bodies = physpace->AllRigidBodies();


      for (auto rigidbody : bodies) {
        if (rigidbody != nullptr)
          rigidbody->Integrate(dt);
      }

			//for (int i = 0; i < bodies.size(); ++i)
			//{

			//	if (bodies[i]->getComponent<RigidBody>() != NULL)
			//	{
			//		bodies[i]->getComponent<RigidBody>()->Integrate(dt);
			//	}
			//	else
			//	{
			//		throw DCException("An object without a RigidBody got into the list of RigidBodies");
			//	}
			//}
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
			auto& bodies = physpace->AllRigidBodies();

      for (auto rigidbody : bodies) {
        if (rigidbody != nullptr)
          rigidbody->PublishResults();
      }

			//for (int i = 0; i < bodies.size(); ++i)
			//{
			//	if (bodies[i]->getComponent<RigidBody>() != NULL)
			//	{
			//		bodies[i]->getComponent<RigidBody>()->PublishResults();
			//	}
			//	else
			//	{
			//		throw DCException("An object without a RigidBody got into the list of RigidBodies");
			//	}
			//}
		}

    /**************************************************************************/
    /*!
    @brief Iterate through all the objects with a 'Transform' component and
    update transforms based on parenting.
    @param A pointer to the 'PhysicsSpace' component.
    */
    /**************************************************************************/
    void Physics::UpdateTransforms(PhysicsSpace *physpace)
    {
      auto objects = dynamic_cast<Space*>(physpace->Owner())->AllObjects();

      for (auto current : *objects)
      {
        current->getComponent<Transform>()->UpdateTranslation();
        current->getComponent<Transform>()->UpdateRotation();
      }
    }

		/**************************************************************************/
		/*!
		@brief Sets up pairs of objects that might be colliding with one another.
		@param A pointer to the 'PhysicsSpace' component.
		*/
		/**************************************************************************/
    std::vector<DetectionPairing> Physics::BroadPhaseDetection(PhysicsSpace* physpace)
		{
			// For all gameobjects with a 'Collider' component
			auto& list = physpace->AllColliders();

      std::vector<DetectionPairing> result;

      DetectionPairing Fill;

			for (int i = 0; i < list.size(); ++i)
			{
				for (int j = i + 1; j < list.size(); ++j)
				{
          auto box1 = dynamic_cast<BoxCollider*>(list[i]);
          auto box2 = dynamic_cast<BoxCollider*>(list[j]);
          auto cir1 = dynamic_cast<CircleCollider*>(list[i]);
          auto cir2 = dynamic_cast<CircleCollider*>(list[j]);

         //auto box1 = list[i]->getComponent<BoxCollider>();
         //auto box2 = list[j]->getComponent<BoxCollider>();
         //auto cir1 = list[i]->getComponent<CircleCollider>();
         //auto cir2 = list[j]->getComponent<CircleCollider>();
         std::string str1, str2;

         

         Fill.obj1 = dynamic_cast<GameObjectPtr>(list[i]->Owner());
         Fill.obj2 = dynamic_cast<GameObjectPtr>(list[j]->Owner());

         if (box1)
         {
           str1 = box1->getCollisionGroup();
         }
         else
         {
           str1 = cir1->getCollisionGroup();
         }

         if (box2)
         {
           str2 = box2->getCollisionGroup();
         }
         else
         {
           str2 = cir2->getCollisionGroup();
         }

         if (str1 == str2)
         {
           Fill.filter = CollisionFilter();
         }

         // need to access the collision table and get info from it
         // Fill.filter = physpace->getCollisionTable()->GetFilter(str1, str2);

         result.push_back(Fill);
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
		void Physics::NarrowPhaseDetection(std::vector<DetectionPairing> pairs, std::vector<Manifold> &contactlist)
		{
			GameObject * obj1, *obj2;

			Manifold collision;
      CollisionData Collision;

			for (auto Pair : pairs)
			{
        obj1 = Pair.obj1;
        obj2 = Pair.obj2;
        Collision.filter = Pair.filter;

        if (Pair.filter.CollisionFlag == CollisionFlag::SkipDetecting)
        {
          continue;
        }

				if (obj1->getComponent<BoxCollider>() && obj2->getComponent<BoxCollider>())
				{
					// COLLISION DETECTED
					if (Collision::BoxtoBox(obj1, obj2, collision))
					{

            if (obj1->getComponent<BoxCollider>()->getGhost() == false && obj2->getComponent<BoxCollider>()->getGhost() == false && Pair.filter.CollisionFlag == CollisionFlag::Resolve && (collision.rigid1 && collision.rigid2))
            {
              contactlist.push_back(collision);
            }
            
            // TEMPORARY: SEND EVENT DIRECTLY TO OBJECTS
						Collision.Object = obj1;
						Collision.OtherObject = obj2;
						DispatchCollisionStarted(Collision);

					}
					// NO COLLISION DETECTED
					else 
          {
						Collision.Object = obj1;
						Collision.OtherObject = obj2;
						DispatchCollisionEnded(Collision);
					}
				}
				else if (obj1->getComponent<CircleCollider>() && obj2->getComponent<CircleCollider>())
				{
					if (Collision::CircletoCircle(obj1, obj2, collision))
					{
            if (obj1->getComponent<CircleCollider>()->getGhost() == false && obj2->getComponent<CircleCollider>()->getGhost() == false && Pair.filter.CollisionFlag == CollisionFlag::Resolve && (collision.rigid1 && collision.rigid2))
            {
              contactlist.push_back(collision);
            }
						// TEMPORARY: SEND EVENT DIRECTLY TO OBJECTS
						Collision.Object = obj1;
						Collision.OtherObject = obj2;
						DispatchCollisionStarted(Collision);

					}
					// NO COLLISION DETECTED
					else
          {
						Collision.Object = obj1;
						Collision.OtherObject = obj2;
						DispatchCollisionEnded(Collision);
					}
				}
				else if ((obj1->getComponent<BoxCollider>() && obj2->getComponent<CircleCollider>()))
				{
					if (Collision::CircletoBox(obj1, obj2, collision))
					{
            if (obj1->getComponent<BoxCollider>()->getGhost() == false && obj2->getComponent<CircleCollider>()->getGhost() == false && Pair.filter.CollisionFlag == CollisionFlag::Resolve && (collision.rigid1 && collision.rigid2))
            {
              contactlist.push_back(collision);
            }
						// TEMPORARY: SEND EVENT DIRECTLY TO OBJECTS
						Collision.Object = obj1;
						Collision.OtherObject = obj2;
						DispatchCollisionStarted(Collision);

					}
					// NO COLLISION DETECTED
					else
					{
						Collision.Object = obj1;
						Collision.OtherObject = obj2;
						DispatchCollisionEnded(Collision);
					}
				}
				else if ((obj1->getComponent<CircleCollider>() && obj2->getComponent<BoxCollider>()))
				{
					if (Collision::CircletoBox(obj2, obj1, collision))
					{
            if (obj1->getComponent<CircleCollider>()->getGhost() == false && obj2->getComponent<BoxCollider>()->getGhost() == false && Pair.filter.CollisionFlag == CollisionFlag::Resolve && (collision.rigid1 && collision.rigid2))
            {
              contactlist.push_back(collision);
            }
						// TEMPORARY: SEND EVENT DIRECTLY TO OBJECTS
						Collision.Object = obj2;
						Collision.OtherObject = obj1;
						DispatchCollisionStarted(Collision);

					}
					// NO COLLISION DETECTED
					else
					{
						Collision.Object = obj2;
						Collision.OtherObject = obj1;
						DispatchCollisionEnded(Collision);
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
      if (collisionData.filter.CollisionStartBlock.SendEventsToA)
      {
        collisionData.Object->Dispatch<Events::CollisionStarted>(collisionStartedEvent);
      }
      // Dispatch collision event to the second object
			collisionStartedEvent->Object = collisionData.OtherObject;
			collisionStartedEvent->OtherObject = collisionData.Object;
      if (collisionData.filter.CollisionStartBlock.SendEventsToB)
      {
        collisionData.OtherObject->Dispatch<Events::CollisionStarted>(collisionStartedEvent);
      }

      delete collisionStartedEvent;
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
      if (collisionData.filter.CollisionStartBlock.SendEventsToA)
      {
        collisionData.Object->Dispatch<Events::CollisionEnded>(collisionEndedEvent);
      }
      // Dispatch collision event to the second object
			collisionEndedEvent->Object = collisionData.OtherObject;
			collisionEndedEvent->OtherObject = collisionData.Object;
      if (collisionData.filter.CollisionStartBlock.SendEventsToB)
      {
        collisionData.OtherObject->Dispatch<Events::CollisionEnded>(collisionEndedEvent);
      }
      delete collisionEndedEvent;
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
