#include "Physics.h"

#include "../../Components/Transform.h"
#include "../../Components/RigidBody.h"
#include "../../Components/BoxCollider.h"
#include "../../Objects/Entities/EntitiesInclude.h"
#include "../../Events/CollisionEvents.h"
#include "Collision.h"
#include "Resolution.h"

namespace DCEngine
{
	void Resolve(float dt, std::vector<Manifold> &contactlist)
	{

		ResolvePositions(dt, contactlist);
		ResolveVelocities(dt, contactlist);

	}

	void ResolveContactVelocity(float dt, Manifold &c)
	{
		//Find the velocity of the two object along the contact normal
		float separatingVelocity = c.CalculateSeparatingVelocity();

		if (separatingVelocity > 0)
		{
			//The objects are no longer moving towards each other
			//or the contact they are stationary
			c.ContactImpulse = 0;
			return;
		}

		//This velocity of this collision has to be resolved.

		//To model inelastic collision the kinetic energy of the bodies 
		//needs to be reduced.
		//This technique uses the combined restitution to determine what percentage
		//of the energy along the collision normal is conserved
		float newSepVelocity = -separatingVelocity * c.Restitution;

		const bool AccelerationBuildUp = true;
		//When an object is resting on the ground it is constantly falling
		//due to gravity. This acceleration need be removed or objects will
		//jitter on the ground.

		if (AccelerationBuildUp)
		{
			// Check the velocity build-up due to acceleration only
			glm::vec3 accCausedVelocity;

			if (c.rigid1 == false && c.rigid2 == false)
			{
				accCausedVelocity = Vec3(0, 0, 0);
			}

			if (c.rigid1 != false && c.rigid2 == false)
			{
				accCausedVelocity = c.Object1->getComponent<RigidBody>()->getAcceleration();
			}

			if (c.rigid1 == false && c.rigid2 != false)
			{
				accCausedVelocity = -c.Object2->getComponent<RigidBody>()->getAcceleration();
			}

			if (c.rigid1 != false && c.rigid2 != false)
			{
				accCausedVelocity = c.Object1->getComponent<RigidBody>()->getAcceleration() - c.Object2->getComponent<RigidBody>()->getAcceleration();
			}


			float accCausedSepVelocity = glm::dot(accCausedVelocity, c.ContactNormal) * dt;

			// If we've got a closing velocity due to acceleration build-up,
			// remove it from the new separating velocity
			if (accCausedSepVelocity < 0)
			{
				newSepVelocity += c.Restitution * accCausedSepVelocity;

				// Make sure we haven't removed more than was
				// there to remove.
				if (newSepVelocity < 0) newSepVelocity = 0;
			}
		}

		//What is the total change in velocity for the contact?
		float deltaVelocity = newSepVelocity - separatingVelocity;

		//The delta velocity is applied to each object proportional to inverse
		//mass. So the more massive an object is the less of the change
		//in velocity it will receive.
		float totalInverseMass;

		if (c.rigid1 == false && c.rigid2 == false)
		{
			totalInverseMass = 0.000000000000000001f;
		}

		if (c.rigid1 != false && c.rigid2 == false)
		{
			totalInverseMass = c.Object1->getComponent<RigidBody>()->getInvMass();
		}

		if (c.rigid1 == false && c.rigid2 != false)
		{
			totalInverseMass = c.Object2->getComponent<RigidBody>()->getInvMass();
		}

		if (c.rigid1 != false && c.rigid2 != false)
		{
			totalInverseMass = c.Object1->getComponent<RigidBody>()->getInvMass() + c.Object2->getComponent<RigidBody>()->getInvMass();
		}


		// Calculate the impulse to apply
		float impulse = deltaVelocity / totalInverseMass * 1.9f;

		c.ContactImpulse = impulse;

		// Find the amount of impulse per unit of inverse mass
		glm::vec3 impulsePerIMass = c.ContactNormal * impulse;

		// Apply impulses: they are applied in the direction of the contact,
		// and in proportion to inverse mass.

		if (c.rigid1 == false && c.rigid2 == false)
		{
			return;
		}

		if (c.rigid1 != false && c.rigid2 == false)
		{
			if (c.Object1->getComponent<RigidBody>()->DynamicState != DynamicStateType::Static)
			{
				// The other body goes in the opposite direction
				c.Object1->getComponent<RigidBody>()->setVelocity(c.Object1->getComponent<RigidBody>()->getVelocity() + impulsePerIMass * c.Object1->getComponent<RigidBody>()->getInvMass());
			}
		}

		if (c.rigid1 == false && c.rigid2 != false)
		{
			if (c.Object2->getComponent<RigidBody>()->DynamicState != DynamicStateType::Static)
			{
				c.Object2->getComponent<RigidBody>()->setVelocity(c.Object2->getComponent<RigidBody>()->getVelocity() + impulsePerIMass * -c.Object2->getComponent<RigidBody>()->getInvMass());
			}
		}

		if (c.rigid1 != false && c.rigid2 != false)
		{
			if (c.Object1->getComponent<RigidBody>()->DynamicState != DynamicStateType::Static)
			{
				c.Object1->getComponent<RigidBody>()->setVelocity(c.Object1->getComponent<RigidBody>()->getVelocity() + impulsePerIMass * c.Object1->getComponent<RigidBody>()->getInvMass());
			}

			if (c.Object2->getComponent<RigidBody>()->DynamicState != DynamicStateType::Static)
			{
				// The other body goes in the opposite direction
				c.Object2->getComponent<RigidBody>()->setVelocity(c.Object2->getComponent<RigidBody>()->getVelocity() + impulsePerIMass * -c.Object2->getComponent<RigidBody>()->getInvMass());
			}
		}


		// this is the code that handles friction
		if (c.rigid1 != false && c.rigid2 == false)
		{
			if (c.Object1->getComponent<RigidBody>()->DynamicState != DynamicStateType::Static)
			{
				// The other body goes in the opposite direction
				Vec3 friction1(-c.Object1->getComponent<RigidBody>()->getVelocity().x, 0, 0);

				friction1 *= c.FrictionCof;

				c.Object1->getComponent<RigidBody>()->setVelocity(c.Object1->getComponent<RigidBody>()->getVelocity() + friction1);
			}
		}

		if (c.rigid1 == false && c.rigid2 != false)
		{
			if (c.Object2->getComponent<RigidBody>()->DynamicState != DynamicStateType::Static)
			{
				Vec3 friction2(-c.Object2->getComponent<RigidBody>()->getVelocity().x, 0, 0);

				friction2 *= c.FrictionCof;

				c.Object2->getComponent<RigidBody>()->setVelocity(c.Object2->getComponent<RigidBody>()->getVelocity() + friction2);
			}
		}

		if (c.rigid1 != false && c.rigid2 != false)
		{
			if (c.Object1->getComponent<RigidBody>()->DynamicState != DynamicStateType::Static)
			{
				Vec3 friction1(-c.Object1->getComponent<RigidBody>()->getVelocity().x, 0, 0);

				friction1 *= c.FrictionCof;

				c.Object1->getComponent<RigidBody>()->setVelocity(c.Object1->getComponent<RigidBody>()->getVelocity() + friction1);
			}

			if (c.Object2->getComponent<RigidBody>()->DynamicState != DynamicStateType::Static)
			{
				// The other body goes in the opposite direction
				Vec3 friction2(-c.Object2->getComponent<RigidBody>()->getVelocity().x, 0, 0);

				friction2 *= c.FrictionCof;

				c.Object2->getComponent<RigidBody>()->setVelocity(c.Object2->getComponent<RigidBody>()->getVelocity() + friction2);
			}
		}

	}

	void ResolveVelocities(float dt, std::vector<Manifold> &contactlist)
	{
		unsigned int iterationsRun = 0;
		unsigned int maxIterations = static_cast<unsigned int>(contactlist.size());
		while (iterationsRun < maxIterations)
		{
			// Find the contact with the largest closing velocity;
			float maxVelocity = FLT_MAX;
			unsigned int contactIndex = static_cast<unsigned int>(contactlist.size());
			for (unsigned i = 0; i < contactlist.size(); ++i)
			{
				float sepVel = contactlist[i].CalculateSeparatingVelocity();
				if (sepVel < 0 && sepVel < maxVelocity)
				{
					maxVelocity = sepVel;
					contactIndex = i;
				}
			}

			// Do we have anything worth resolving?
			if (contactIndex == contactlist.size())
			{
				break;
			}

			// Resolve this contact velocity
			ResolveContactVelocity(dt, contactlist[contactIndex]);

			++iterationsRun;
		}
	}

	void ResolvePenetration(float dt, Manifold& c)
	{
		// The movement of each object is based on their inverse mass, so
		// total that.
		float totalInverseMass;

		if (c.rigid1 == false && c.rigid2 == false)
		{
			totalInverseMass = 0.000000000000000001f;
		}

		if (c.rigid1 != false && c.rigid2 == false)
		{
			totalInverseMass = c.Object1->getComponent<RigidBody>()->getInvMass();
		}

		if (c.rigid1 == false && c.rigid2 != false)
		{
			totalInverseMass = c.Object2->getComponent<RigidBody>()->getInvMass();
		}

		if (c.rigid1 != false && c.rigid2 != false)
		{
			totalInverseMass = c.Object1->getComponent<RigidBody>()->getInvMass() + c.Object2->getComponent<RigidBody>()->getInvMass();
		}

		// Find the amount of penetration resolution per unit of inverse mass
		glm::vec3 movePerIMass = c.ContactNormal * (c.Penetration / totalInverseMass);

		//If stack stability can be increased by not resolving all the penetrations
		//in one step
		movePerIMass *= 0.2f;

		// Calculate the the movement amounts

		if (c.rigid1 == false && c.rigid2 == false)
		{
			c.v1 = movePerIMass *  0.0f;
			c.v2 = movePerIMass * -0.0f;
		}

		if (c.rigid1 != false && c.rigid2 == false)
		{
			c.v1 = movePerIMass *  c.Object1->getComponent<RigidBody>()->getInvMass();
			c.v2 = movePerIMass * -0.0f;
		}

		if (c.rigid1 == false && c.rigid2 != false)
		{
			c.v1 = movePerIMass *  0.0f;
			c.v2 = movePerIMass * -c.Object2->getComponent<RigidBody>()->getInvMass();
		}

		if (c.rigid1 != false && c.rigid2 != false)
		{
			c.v1 = movePerIMass *  c.Object1->getComponent<RigidBody>()->getInvMass();
			c.v2 = movePerIMass * -c.Object2->getComponent<RigidBody>()->getInvMass();
		}

		// Apply the penetration resolution

		if (c.rigid1 == false && c.rigid2 == false)
		{
			return;
		}

		if (c.rigid1 != false && c.rigid2 == false)
		{
			if (c.Object1->getComponent<RigidBody>()->DynamicState != DynamicStateType::Static)
			{
				c.Object1->getComponent<RigidBody>()->setPosition(c.Object1->getComponent<RigidBody>()->getPosition() + c.v1);
			}
		}

		if (c.rigid1 == false && c.rigid2 != false)
		{
			if (c.Object2->getComponent<RigidBody>()->DynamicState != DynamicStateType::Static)
			{
				c.Object2->getComponent<RigidBody>()->setPosition(c.Object2->getComponent<RigidBody>()->getPosition() + c.v2);
			}
		}

		if (c.rigid1 != false && c.rigid2 != false)
		{
			if (c.Object1->getComponent<RigidBody>()->DynamicState != DynamicStateType::Static)
			{
				c.Object1->getComponent<RigidBody>()->setPosition(c.Object1->getComponent<RigidBody>()->getPosition() + c.v1);
			}

			if (c.Object2->getComponent<RigidBody>()->DynamicState != DynamicStateType::Static)
			{
				c.Object2->getComponent<RigidBody>()->setPosition(c.Object2->getComponent<RigidBody>()->getPosition() + c.v2);
			}
		}


	}

	void ResolvePositions(float dt, std::vector<Manifold> &contactlist)
	{
		//Resolving penetrations is a tricky problem with large stacks. The only real way to get 
		//good results is by considering all contacts at once at solving them together. This method basically 
		//just keeps poking the bodies until the penetrations are below the penetration slop epsilon. 
		//Better solutions involve building contact graphs, shock propagation and constraint based
		//solvers.
		unsigned int iterationsRun = 0;
		unsigned int maxIterations = static_cast<unsigned int>(contactlist.size() * 5);
		const float positionEpsilon = 0.0001f;

		while (iterationsRun < maxIterations)
		{
			// Find biggest penetration greater than
			//the correction epsilon
			float maxPenetration = positionEpsilon;
			unsigned contactIndex = static_cast<unsigned int>(contactlist.size());
			for (unsigned j = 0; j < contactlist.size(); ++j)
			{
				if (contactlist[j].Penetration > maxPenetration)
				{
					maxPenetration = contactlist[j].Penetration;
					contactIndex = j;
				}
			}

			if (contactIndex >= contactlist.size())
			{
				break;
			}

			//Resolve the penetration
			ResolvePenetration(dt, contactlist[contactIndex]);

			++iterationsRun;
		}
	}

}
