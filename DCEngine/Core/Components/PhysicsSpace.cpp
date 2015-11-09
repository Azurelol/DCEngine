#include "PhysicsSpace.h"
#include "EngineReference.h"
#include "../../Core/Systems/Physics/Physics.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  \brief Constructor for the 'PhysisSpace' component.
  */
  /**************************************************************************/
  PhysicsSpace::PhysicsSpace(Entity& owner) : Component(std::string("PhysicsSpace"), owner) {

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


  void PhysicsSpace::Serialize(Json::Value & root)
  {

  }

  void PhysicsSpace::Deserialize(Json::Value & root)
  {

  }

  /**************************************************************************/
  /*!
  \brief Adds a GameObject with a 'RigidBody' component.
  \param A pointer to the 'GameObject'
  */
  /**************************************************************************/
  void PhysicsSpace::AddRigidBody(GameObject* rigidbody)
  {
    rigidbodies_.push_back(rigidbody);
    DCTrace << "PhysicsSpace::AddRigidBody - " << rigidbody->Name() << "\n";
  }

  /**************************************************************************/
  /*!
  \brief Adds a GameObject with a 'BoxCollider' component.
  \param A pointer to the 'GameObject'
  */
  /**************************************************************************/
  void PhysicsSpace::AddCollider(GameObject* collider)
  {
    colliders_.push_back(collider);
    DCTrace << "PhysicsSpace::AddCollider - " << collider->Name() << "\n";
  }

  /**************************************************************************/
  /*!
  \brief Returns the container of 'GameObjects' with 'Rigidbody' component.
  \return A container of GameObjects.
  */
  /**************************************************************************/
  GameObjectRawVec PhysicsSpace::getRigidbodies()
  {
    return rigidbodies_;
  }
  /**************************************************************************/
  /*!
  \brief Returns the container of 'GameObjects' with 'Collider' component.
  \return A container of GameObjects.
  */
  /**************************************************************************/
  GameObjectRawVec PhysicsSpace::getColliders()
  {
    return colliders_;
  }
}