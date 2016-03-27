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
#include "../../Systems/Physics/QuadTree.h"
// Custom physics libraries

#include "Resolution.h"

namespace DCEngine {
  namespace Systems {
    /**************************************************************************/
    /*!
    \brief Constructor for the Physics system.
    */
    /**************************************************************************/
    Physics::Physics() : System(std::string("PhysicsSystem"), EnumeratedSystem::Physics),
      minX(10000), minY(10000), maxX(-10000), maxY(-10000) {
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
    void Physics::RegisterSpace(Components::PhysicsSpace & physicsSpace)
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
    void Physics::DeregisterSpace(Components::PhysicsSpace & physicsSpace)
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
      SystemTimer profile(this->Name());

      // Do not update physics while the engine is paused.
      if (Paused)
        return;

        Step(dt);
      
    }

    /**************************************************************************/
    /*!
    @brief Checks if the GameObject is within the bounding area.
    @param center The center of the bounding box.
    @param width The width of the bounding box.
    @param height The height of the bounding box.
    @param gameObject The specified GameObject.
    @return Whether the GameObject was found within the bounding area.
    */
    /**************************************************************************/
    bool Physics::IsObjectWithinBoundingArea(Vec3 & center, float width, float height, GameObjectPtr gameObject)
    {
      if (!gameObject->HasComponent("Transform"))
        return false;

      auto& translation = gameObject->getComponent<Components::Transform>()->getTranslation();
      auto& scale = gameObject->getComponent<Components::Transform>()->getScale();
      auto& rotation = gameObject->getComponent<Components::Transform>()->getRotation();
      return Collision::SelectiontoBox(center, width, height, translation, scale, rotation.z);
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
    @todo Pass the desired FPS to the Physics system in a struct.
    */
    /**************************************************************************/
    void Physics::Step(float dt)
    {
      // Hardcode dt temporarily
      dt = 0.0166f;
      static bool first = true;
      // Iterate through every space that has the 'PhysicsSpace' component
      for (auto physpace : physicsSpaces_)
      {
        // If the space is paused, skip physics update
        if (physpace->Owner()->getComponent<Components::TimeSpace>()->getPaused())
          continue;

        
        if (first)
        {
          UpdateTransforms(physpace);
        }

        std::vector<Manifold> contactlist;
        static std::vector<DetectionPairing> pairs;

        BroadPhaseDetection(physpace, pairs);

        Integrate(dt, physpace);

        NarrowPhaseDetection(pairs, contactlist);

        Resolution::Resolve(dt, contactlist);

        PublishResults(physpace);

        UpdateTransforms(physpace);
      }

      first = false;

    }

    /**************************************************************************/
    /*!
    @brief  Iterate through all the rigid bodies and update velocity.
    @param  The delta time.
    @param A pointer to the 'PhysicsSpace' component.
    */
    /**************************************************************************/
    void Physics::Integrate(float dt, Components::PhysicsSpace* physpace)
    {
      SystemMethodTimer timer("Integrate", EnumeratedSystem::Physics);
      auto& bodies = physpace->AllRigidBodies();


      for (auto rigidbody : bodies) 
      {
        if (rigidbody != nullptr)
        {
          rigidbody->Integrate(dt);
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
    void Physics::PublishResults(Components::PhysicsSpace* physpace)
    {
      SystemMethodTimer timer("PublishResults", EnumeratedSystem::Physics);
      auto& bodies = physpace->AllRigidBodies();

      for (auto rigidbody : bodies) {
        if (rigidbody != nullptr)
          rigidbody->PublishResults();
      }
    }

    /**************************************************************************/
    /*!
    @brief Iterate through all the objects with a 'Transform' component and
    update transforms based on parenting.
    @param physpace A pointer to the 'PhysicsSpace' component.
    */
    /**************************************************************************/
    void Physics::UpdateTransforms(Components::PhysicsSpace *physpace)
    {
      SystemMethodTimer timer("UpdateTransforms", EnumeratedSystem::Physics);
      auto objects = dynamic_cast<Space*>(physpace->Owner())->AllObjects();

      for (auto current : *objects)
      {
        // If it doesn't have a transform...
        if (!current->HasComponent("Transform"))
          continue;

        current->getComponent<Components::Transform>()->UpdateTranslation();
        current->getComponent<Components::Transform>()->UpdateRotation();
      }
    }

    /**************************************************************************/
    /*!
    @brief Sets up pairs of objects that might be colliding with one another.
    @param A pointer to the 'PhysicsSpace' component.
    */
    /**************************************************************************/
    void Physics::BroadPhaseDetection(Components::PhysicsSpace* physpace, std::vector<DetectionPairing> &pairs)
    {
      // For all gameobjects with a 'Collider' component
      SystemMethodTimer timer("BroadPhaseDetection", EnumeratedSystem::Physics);
      Components::ColliderContainer& list = physpace->AllColliders();

      //find extent of all objects
      for (auto collider : list)
      {
        Vec3 min;
        Vec3 max;
        Components::Transform* transform = collider->Owner()->getComponent<Components::Transform>();
        Components::BoxCollider* box = collider->Owner()->getComponent<Components::BoxCollider>();
        if (box)
        {
          min = transform->Translation - box->getColliderScale() + box->Offset;
          max = transform->Translation + box->getColliderScale() + box->Offset;
        }
        else
        {
          Components::CircleCollider* circle = collider->Owner()->getComponent<Components::CircleCollider>();
          if (circle)
          {
            min = transform->Translation - transform->Scale * circle->Radius + box->Offset;
            max = transform->Translation - transform->Scale * circle->Radius + box->Offset;
          }
        }
        if (min.x < minX)
          minX = min.x;
        if (min.y < minY)
          minY = min.y;
        if (max.x > maxX)
          maxX = max.x;
        if (max.y > maxY)
          maxY = max.y;

      }

      QuadTree qt(list, Vec2(minX, minY), Vec2(maxX, maxY), QuadTreeBucketSize);
      pairs = qt.CreatePairs();

      Components::BoxCollider*    box1 = NULL;
      Components::BoxCollider*    box2 = NULL;
      Components::CircleCollider* cir1 = NULL;
      Components::CircleCollider* cir2 = NULL;

      bool rigid1 = false, rigid2 = false;
      
      std::string str1, str2;


      for (auto &pair : pairs)
      {
     
         auto rigidbody1 = pair.obj1->getComponent<Components::RigidBody>();
         auto rigidbody2 = pair.obj2->getComponent<Components::RigidBody>();
     
         if (rigidbody1 == NULL)
         {
           rigid1 = false;
         }
         else
         {
           rigid1 = true;
     
           if (rigidbody1->getDynamicState() == DynamicStateType::Static)
           {
             rigid1 = false;
           }
         }
     
         if (rigidbody2 == NULL)
         {
           rigid2 = false;
         }
         else
         {
           rigid2 = true;
     
           if (rigidbody2->getDynamicState() == DynamicStateType::Static)
           {
             rigid2 = false;
           }
         }
     
         if (!rigid1 && !rigid2)
         {
           continue;
         }
     
         box1 = pair.obj1->getComponent<Components::BoxCollider>();
         box2 = pair.obj2->getComponent<Components::BoxCollider>();
         cir1 = pair.obj1->getComponent<Components::CircleCollider>();
         cir2 = pair.obj2->getComponent<Components::CircleCollider>();
     
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

         pair.filter = Daisy->getSystem<Content>()->getCollisionTable(std::string(physpace->getCollisionTable()))->GetFilter(str1, str2);
      }


      //static int listsize = 0;
      //
      //if (list.size() == listsize)
      //{
      //  //return;
      //}
      //
      //listsize = list.size();
      //
      //pairs.clear();
      //
      //Components::BoxCollider*    box1 = NULL;
      //Components::BoxCollider*    box2 = NULL;
      //Components::CircleCollider* cir1 = NULL;
      //Components::CircleCollider* cir2 = NULL;
      //int count = 0;
      //
      //bool rigid1 = false, rigid2 = false;
      //
      //std::string str1, str2;
      //
      //DetectionPairing Fill;
      //
      ////pairs.resize(list.size() * list.size());
      //
      //for (int i = 0; i < list.size(); ++i)
      //{
      //  for (int j = i + 1; j < list.size(); ++j)
      //  {
      //    Fill.obj1 = static_cast<GameObjectPtr>(list[i]->Owner());
      //    Fill.obj2 = static_cast<GameObjectPtr>(list[j]->Owner());
      //
      //    auto rigidbody1 = Fill.obj1->getComponent<Components::RigidBody>();
      //    auto rigidbody2 = Fill.obj2->getComponent<Components::RigidBody>();
      //
      //    if (rigidbody1 == NULL)
      //    {
      //      rigid1 = false;
      //    }
      //    else
      //    {
      //      rigid1 = true;
      //
      //      if (rigidbody1->getDynamicState() == DynamicStateType::Static)
      //      {
      //        rigid1 = false;
      //      }
      //    }
      //
      //    if (rigidbody2 == NULL)
      //    {
      //      rigid2 = false;
      //    }
      //    else
      //    {
      //      rigid2 = true;
      //
      //      if (rigidbody2->getDynamicState() == DynamicStateType::Static)
      //      {
      //        rigid2 = false;
      //      }
      //    }
      //
      //    if (!rigid1 && !rigid2)
      //    {
      //      continue;
      //    }
      //
      //    box1 = Fill.obj1->getComponent<Components::BoxCollider>();
      //    box2 = Fill.obj2->getComponent<Components::BoxCollider>();
      //    cir1 = Fill.obj1->getComponent<Components::CircleCollider>();
      //    cir2 = Fill.obj2->getComponent<Components::CircleCollider>();
      //
      //    if (box1)
      //    {
      //      str1 = box1->getCollisionGroup();
      //    }
      //    else
      //    {
      //      str1 = cir1->getCollisionGroup();
      //    }
      //
      //    if (box2)
      //    {
      //      str2 = box2->getCollisionGroup();
      //    }
      //    else
      //    {
      //      str2 = cir2->getCollisionGroup();
      //    }
      //
      //    if (str1 == str2)
      //    {
      //      Fill.filter = CollisionFilter();
      //    }
      //    else
      //    {
      //      // need to access the collision table and get info from it
      //      //Fill.filter = Daisy->getSystem<Content>()->getCollisionTable(std::string(physpace->getCollisionTable()))->GetFilter(str1, str2);
      //    }
      //    pairs.push_back(Fill);
      //  }
      //}
    }

    /**************************************************************************/
    /*!
    @brief Detect if collision is happening and store information about it.
    @param A vector of GameObjects.
    */
    /**************************************************************************/
    void Physics::NarrowPhaseDetection(std::vector<DetectionPairing> &pairs, std::vector<Manifold> &contactlist)
    {
      SystemMethodTimer timer("NarrowPhaseDetection", EnumeratedSystem::Physics);
      GameObject * obj1, *obj2;
      std::pair<GameObjectPtr, GameObjectPtr> pair;
      Manifold collision;
      CollisionData Collision;

      for (auto Pair : pairs)
      {
        obj1 = Pair.obj1;
        obj2 = Pair.obj2;
        Collision.filter = Pair.filter;

        pair.first = obj1;
        pair.second = obj2;

        if (Pair.filter.CollisionFlag == CollisionFlag::SkipDetecting)
        {
          continue;
        }

        if (obj1->getComponent<Components::BoxCollider>() && obj2->getComponent<Components::BoxCollider>())
        {
          // COLLISION DETECTED
          if (Collision::BoxtoBox(obj1, obj2, collision))
          {
            
            // TEMPORARY: SEND EVENT DIRECTLY TO OBJECTS
            Collision.Object = obj1;
            Collision.OtherObject = obj2;
            Collision.Normal = collision.ContactNormal;
            if (!Persisted(pair))
            {
              DispatchCollisionStarted(Collision);
              PersistedPairs.push_back(pair);
            }
            else
            {
              collision.FrictionCof *= 0.9f;
              DispatchCollisionPersisted(Collision);
            }

            if (obj1->getComponent<Components::BoxCollider>()->getGhost() == false && obj2->getComponent<Components::BoxCollider>()->getGhost() == false && Pair.filter.CollisionFlag == CollisionFlag::Resolve)
            {
              contactlist.push_back(collision);
            }

          }
          // NO COLLISION DETECTED
          else 
          {
            Collision.Object = obj1;
            Collision.OtherObject = obj2;
            if (Persisted(pair))
            {
              DispatchCollisionEnded(Collision);
              RemovePair(pair);
            }
          }
        }
        else if (obj1->getComponent<Components::CircleCollider>() && obj2->getComponent<Components::CircleCollider>())
        {
          if (Collision::CircletoCircle(obj1, obj2, collision))
          {

            // TEMPORARY: SEND EVENT DIRECTLY TO OBJECTS
            Collision.Object = obj1;
            Collision.OtherObject = obj2;
            Collision.Normal = collision.ContactNormal;
            if (!Persisted(pair))
            {
              DispatchCollisionStarted(Collision);
              PersistedPairs.push_back(pair);
            }
            else
            {
              collision.FrictionCof *= 0.9f;
              DispatchCollisionPersisted(Collision);
            }

            if (obj1->getComponent<Components::CircleCollider>()->getGhost() == false && obj2->getComponent<Components::CircleCollider>()->getGhost() == false && Pair.filter.CollisionFlag == CollisionFlag::Resolve)
            {
              contactlist.push_back(collision);
            }

          }
          // NO COLLISION DETECTED
          else
          {
            Collision.Object = obj1;
            Collision.OtherObject = obj2;
            if (Persisted(pair))
            {
              DispatchCollisionEnded(Collision);
              RemovePair(pair);
            }
          }
        }
        else if ((obj1->getComponent<Components::BoxCollider>() && obj2->getComponent<Components::CircleCollider>()))
        {
          if (Collision::CircletoBox(obj1, obj2, collision))
          {

            // TEMPORARY: SEND EVENT DIRECTLY TO OBJECTS
            Collision.Object = obj1;
            Collision.OtherObject = obj2;
            Collision.Normal = collision.ContactNormal;
            if (!Persisted(pair))
            {
              DispatchCollisionStarted(Collision);
              PersistedPairs.push_back(pair);
            }
            else
            {
              collision.FrictionCof *= 0.9f;
              DispatchCollisionPersisted(Collision);
            }

            if (obj1->getComponent<Components::BoxCollider>()->getGhost() == false && obj2->getComponent<Components::CircleCollider>()->getGhost() == false && Pair.filter.CollisionFlag == CollisionFlag::Resolve)
            {
              contactlist.push_back(collision);
            }

          }
          // NO COLLISION DETECTED
          else
          {
            Collision.Object = obj1;
            Collision.OtherObject = obj2;
            if (Persisted(pair))
            {
              DispatchCollisionEnded(Collision);
              RemovePair(pair);
            }
          }
        }
        else if ((obj1->getComponent<Components::CircleCollider>() && obj2->getComponent<Components::BoxCollider>()))
        {
          if (Collision::CircletoBox(obj2, obj1, collision))
          {

            // TEMPORARY: SEND EVENT DIRECTLY TO OBJECTS
            Collision.Object = obj2;
            Collision.OtherObject = obj1;
            Collision.Normal = collision.ContactNormal;
            if (!Persisted(pair))
            {
              DispatchCollisionStarted(Collision);
              PersistedPairs.push_back(pair);
            }
            else
            {
              collision.FrictionCof *= 0.9f;
              DispatchCollisionPersisted(Collision);
            }

            if (obj1->getComponent<Components::CircleCollider>()->getGhost() == false && obj2->getComponent<Components::BoxCollider>()->getGhost() == false && Pair.filter.CollisionFlag == CollisionFlag::Resolve)
            {
              contactlist.push_back(collision);
            }

          }
          // NO COLLISION DETECTED
          else
          {
            Collision.Object = obj2;
            Collision.OtherObject = obj1;
            if (Persisted(pair))
            {
              DispatchCollisionEnded(Collision);
              RemovePair(pair);
            }
          }
        }
      }
      return;
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
      collisionStartedEvent->Normal = collisionData.Normal;
      if (collisionData.filter.CollisionStartBlock.SendEventsToA)
      {
        collisionData.Object->Dispatch<Events::CollisionStarted>(collisionStartedEvent);
      }
      // Dispatch collision event to the second object
      collisionStartedEvent->Object = collisionData.OtherObject;
      collisionStartedEvent->OtherObject = collisionData.Object;
      collisionStartedEvent->Normal = -collisionData.Normal;
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
    @brief Dispatch the event that signals that a collision is persisting
           between two objects.
    @param A struct that contains the collision data detected by the engine.
    */
    /**************************************************************************/
    void Physics::DispatchCollisionPersisted(CollisionData & collisionData)
    {
      auto collisionPersistedEvent = new Events::CollisionPersisted();
      // Dispatch collision event to the first object
      collisionPersistedEvent->Object = collisionData.Object;
      collisionPersistedEvent->OtherObject = collisionData.OtherObject;
      collisionPersistedEvent->Normal = collisionData.Normal;
      if (collisionData.filter.CollisionStartBlock.SendEventsToA)
      {
        collisionData.Object->Dispatch<Events::CollisionPersisted>(collisionPersistedEvent);
      }
      // Dispatch collision event to the second object
      collisionPersistedEvent->Object = collisionData.OtherObject;
      collisionPersistedEvent->OtherObject = collisionData.Object;
      collisionPersistedEvent->Normal = -collisionData.Normal;
      if (collisionData.filter.CollisionStartBlock.SendEventsToB)
      {
        collisionData.OtherObject->Dispatch<Events::CollisionPersisted>(collisionPersistedEvent);
      }
      delete collisionPersistedEvent;
    }


    /**************************************************************************/
    /*!
    \brief Terminate the physics system.
    */
    /**************************************************************************/
    void Physics::Terminate() {
    }

    /**************************************************************************/
    /*!
    @brief determine if an object pairing was colliding last frame.
    @param A std::pair of pointers.
    */
    /**************************************************************************/
    bool Physics::Persisted(std::pair<GameObjectPtr, GameObjectPtr> &pair)
    {
      for (auto Pair : PersistedPairs)
      {
        if ((Pair.first == pair.first && Pair.second == pair.second) || (Pair.first == pair.second && Pair.second == pair.first))
        {
          return true;
        }
      }
      return false;
    }

    void Physics::RemovePair(std::pair<GameObjectPtr, GameObjectPtr> &pair)
    {
      for (auto i = PersistedPairs.begin(); i != PersistedPairs.end(); ++i)
      {
        if ((i._Ptr->first == pair.first && i._Ptr->second == pair.second) || (i._Ptr->first == pair.second && i._Ptr->second == pair.first))
        {
          PersistedPairs.erase(i);
          return;
        }
      }
    }
  }
}
