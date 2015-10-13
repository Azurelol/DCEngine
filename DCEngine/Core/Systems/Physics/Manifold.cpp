#pragma once
#include "Manifold.h"

namespace DCEngine 
{

	float Manifold::CalculateSeparatingVelocity()
  {
    glm::vec3 relativeVelocity = Object1->getComponent<RigidBody>()->getVelocity() - Object2->getComponent<RigidBody>()->getVelocity();

    //Get the separating velocity by projecting along the contact normal
    SeperatingVelocity = glm::dot(relativeVelocity, ContactNormal);

    return SeperatingVelocity;
	}


}