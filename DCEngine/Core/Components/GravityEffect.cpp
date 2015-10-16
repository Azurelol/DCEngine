#pragma once
#include "GravityEffect.h"
#include "PhysicsSpace.h"
#include "EngineReference.h"

namespace DCEngine 
{
  void GravityEffect::Update(float dt)
  {
    if (Active)
    {
      RigidBody *rigidbody = this->Owner()->getComponent<RigidBody>();
      
      if (rigidbody != NULL)
      {
        //Determine the acceleration
        Real3 Acceleration = PHYSICS::Gravity * Direction;

        // Integrate the velocity
        rigidbody->setVelocity(rigidbody->getVelocity() + Acceleration * dt);
      }
      else
      {
        throw DCException("Gravity Component on object with no RigidBody");
      }

    }

  }
}