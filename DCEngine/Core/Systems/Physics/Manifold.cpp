#pragma once
#include "Manifold.h"
// Components
#include "../../Components/RigidBody.h"
#include "../../Objects/Entities/GameObject.h"

namespace DCEngine
{

	float Manifold::CalculateSeparatingVelocity()
	{
		glm::vec3 relativeVelocity;

		if (this->rigid1 == false && this->rigid2 == false)
		{
			relativeVelocity = Vec3(0, 0, 0);
		}

		if (this->rigid1 != false && this->rigid2 == false)
		{
			relativeVelocity = Object1->getComponent<RigidBody>()->getVelocity();
		}

		if (this->rigid1 == false && this->rigid2 != false)
		{
			relativeVelocity = -Object2->getComponent<RigidBody>()->getVelocity();
		}

		if (this->rigid1 != false && this->rigid2 != false)
		{
			relativeVelocity = Object1->getComponent<RigidBody>()->getVelocity() - Object2->getComponent<RigidBody>()->getVelocity();
		}

		//Get the separating velocity by projecting along the contact normal
		SeperatingVelocity = glm::dot(relativeVelocity, ContactNormal);

		return SeperatingVelocity;
	}


}