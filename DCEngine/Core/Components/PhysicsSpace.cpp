#include "PhysicsSpace.h"
#include "EngineReference.h"
#include "../../Core/Systems/Physics/Physics.h"

namespace DCEngine {

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
  void PhysicsSpace::AddRigidBody(GameObject* rigidbody)
  {
    RigidBodiesContainer.push_back(rigidbody);
    DCTrace << "PhysicsSpace::AddRigidBody - " << rigidbody->Name() << "\n";
  }

  /**************************************************************************/
  /*!
  \brief Remvoes a GameObject with a 'RigidBody' component.
  \param A pointer to the 'GameObject'
  */
  /**************************************************************************/
  void PhysicsSpace::RemoveRigidBody(GameObject * rigidbody)
  {
    RigidBodiesContainer.erase(std::remove(RigidBodiesContainer.begin(),
                                           RigidBodiesContainer.end(), rigidbody),
                                           RigidBodiesContainer.end());
  }

  /**************************************************************************/
  /*!
  \brief Adds a GameObject with a 'BoxCollider' component.
  \param A pointer to the 'GameObject'
  */
  /**************************************************************************/
  void PhysicsSpace::AddCollider(GameObject* collider)
  {
    CollidersContainer.push_back(collider);
    DCTrace << "PhysicsSpace::AddCollider - " << collider->Name() << "\n";
  }

  /**************************************************************************/
  /*!
  \brief Remvoes a GameObject with a 'RigidBody' component.
  \param A pointer to the 'GameObject'
  */
  /**************************************************************************/
  void PhysicsSpace::RemoveCollider(GameObject * collider)
  {
    CollidersContainer.erase(std::remove(CollidersContainer.begin(),
                                         CollidersContainer.end(), collider),
                                         CollidersContainer.end());
  }

  /**************************************************************************/
  /*!
  \brief Returns the container of 'GameObjects' with 'Rigidbody' component.
  \return A container of GameObjects.
  */
  /**************************************************************************/
  GameObjectRawVec PhysicsSpace::getRigidbodies()
  {
    return RigidBodiesContainer;
  }
  /**************************************************************************/
  /*!
  \brief Returns the container of 'GameObjects' with 'Collider' component.
  \return A container of GameObjects.
  */
  /**************************************************************************/
  GameObjectRawVec PhysicsSpace::getColliders()
  {
    return CollidersContainer;
  }
}