#include "Physics.h"

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
    void Physics::Initialize() {

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
      for (auto physpace : physicsSpaces_) {

        // For all gameobjects with a 'RigidBody' component
        for (auto rb : physpace->getRigidbodies()) {
          
        }

        // For all gameobjects with a 'BoxCollider' component
        for (auto bc : physpace->getColliders()) {

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
    \brief re
    */
    /**************************************************************************/
    void Physics::BroadPhaseDetection()
    {
    }

    /**************************************************************************/
    /*!
    \brief mi
    */
    /**************************************************************************/
    void Physics::NarrowPhaseDetection()
    {
    }

    /**************************************************************************/
    /*!
    \brief so
    */
    /**************************************************************************/
    void Physics::Resolve()
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