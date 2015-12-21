/******************************************************************************/
/*!
\file   Manifold.cpp
\author Blaine Reiner
\par    email: blaine.reiner\@digipen.edu
\par    DigiPen login: blaine.reiner
\date   11/20/2015
\brief  Implementation of the Manifold class
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
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
			relativeVelocity = Object1->getComponent<Components::RigidBody>()->getVelocity();
		}

		if (this->rigid1 == false && this->rigid2 != false)
		{
			relativeVelocity = -Object2->getComponent<Components::RigidBody>()->getVelocity();
		}

		if (this->rigid1 != false && this->rigid2 != false)
		{
			relativeVelocity = Object1->getComponent<Components::RigidBody>()->getVelocity() - Object2->getComponent<Components::RigidBody>()->getVelocity();
		}

		//Get the separating velocity by projecting along the contact normal
		SeperatingVelocity = glm::dot(relativeVelocity, ContactNormal);

		return SeperatingVelocity;
	}


}