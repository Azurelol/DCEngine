/*****************************************************************************/
/*!
\file   PhysicSpaces.cpp
\author Blaine Reiner, Christian Sagel
\par    email: blaine.reiner@digipen.edu, c.sagel\@digipen.edu
\date   9/23/2015
\brief  The physics space hold the settings for all physical interactions within
a space. It also holds several containers of objects that require
physics integration and resolution, for collisions, velocity, etc...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "PhysicsSpace.h"
#include "EngineReference.h"
#include "../../Core/Systems/Physics/Physics.h"
#include "../Resources/CollisionTable.h"

namespace DCEngine {
  namespace Components {
    /**************************************************************************/
/*!
\brief PhysicsSpace constructor.
*/
/**************************************************************************/
    PhysicsSpace::PhysicsSpace(Entity& owner) : Component(std::string("PhysicsSpace"), owner) {

    }

    /**************************************************************************/
    /*!
    @brief PhysicsSpace destructor.
    */
    /**************************************************************************/
    PhysicsSpace::~PhysicsSpace()
    {
      // Deregister from the Physics System
      Daisy->getSystem<Systems::Physics>()->DeregisterSpace(*this);
    }

    /**************************************************************************/
    /*!
    \brief Initializes the Physics Space. The presence of a physis space component
           signals to the Physics system that the space requires physics resolution.
    */
    /**************************************************************************/
    void PhysicsSpace::Initialize() {
      // Register this space to the physics system
      Daisy->getSystem<Systems::Physics>()->RegisterSpace(*this);
    }

    /**************************************************************************/
    /*!
    \brief Adds a GameObject with a 'RigidBody' component.
    \param A pointer to the 'GameObject'
    */
    /**************************************************************************/
    void PhysicsSpace::AddRigidBody(RigidBody& rigidbody)
    {
      RigidBodiesContainer.push_back(&rigidbody);
      DCTrace << Owner()->getObjectName()
        << "::PhysicsSpace::AddRigidBody - " << rigidbody.Owner()->Name() << "\n";
    }

    /**************************************************************************/
    /*!
    \brief Remvoes a GameObject with a 'RigidBody' component.
    \param A pointer to the 'GameObject'
    */
    /**************************************************************************/
    void PhysicsSpace::RemoveRigidBody(RigidBody& rigidbody)
    {
      DCTrace << Owner()->getObjectName()
        << "::PhysicsSpace::RemoveRigidBody - " << rigidbody.Owner()->Name() << "\n";
      RigidBody* ptr = &rigidbody;
      RigidBodiesContainer.erase(std::remove(RigidBodiesContainer.begin(),
        RigidBodiesContainer.end(), ptr),
        RigidBodiesContainer.end());
    }

    /**************************************************************************/
    /*!
    \brief Adds a GameObject with a 'BoxCollider' component.
    \param A pointer to the 'GameObject'
    */
    /**************************************************************************/
    void PhysicsSpace::AddCollider(Collider* collider)
    {
      CollidersContainer.push_back(collider);
      DCTrace << Owner()->Name() << "::PhysicsSpace::AddCollider - "
        << collider->Owner()->Name() << "\n";
    }

    /**************************************************************************/
    /*!
    \brief Removes a GameObject with a 'RigidBody' component.
    \param A pointer to the 'GameObject'
    */
    /**************************************************************************/
    void PhysicsSpace::RemoveCollider(Collider* collider)
    {
      DCTrace << Owner()->Name() << "::PhysicsSpace::RemoveCollider - "
        << collider->Owner()->Name() << "\n";
      //BoxCollider* ptr = collider;
      CollidersContainer.erase(std::remove(CollidersContainer.begin(),
        CollidersContainer.end(), collider),
        CollidersContainer.end());
    }

    /**************************************************************************/
    /*!
    @brief Returns the container of active RigidBodies.
    @return A container of GameObjects.
    */
    /**************************************************************************/
    RigidBodyContainer& PhysicsSpace::AllRigidBodies()
    {
      return RigidBodiesContainer;
    }
    /**************************************************************************/
    /*!
    @brief Returns the container of active Colliders.
    @return A container of GameObjects.
    */
    /**************************************************************************/
    ColliderContainer& PhysicsSpace::AllColliders()
    {
      return CollidersContainer;
    }

    /**************************************************************************/
    /*!
    @brief Sets the collision table.
    */
    /**************************************************************************/
    void PhysicsSpace::setCollisionTable(CollisionTable & table)
    {
      Table = table;
    }



    /**************************************************************************/
    /*!
    @brief Gets the collision table.
    */
    /**************************************************************************/
    CollisionTable *PhysicsSpace::getCollisionTable(void)
    {
      return &Table;
    }

  }
}