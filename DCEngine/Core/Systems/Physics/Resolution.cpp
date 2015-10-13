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
      glm::vec3 accCausedVelocity = c.Object1->getComponent<RigidBody>()->getAcceleration() - c.Object2->getComponent<RigidBody>()->getAcceleration();
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
    float totalInverseMass = c.Object1->getComponent<RigidBody>()->getInvMass() + c.Object2->getComponent<RigidBody>()->getInvMass();

    // Calculate the impulse to apply
    float impulse = deltaVelocity / totalInverseMass;

    c.ContactImpulse = impulse;

    // Find the amount of impulse per unit of inverse mass
    glm::vec3 impulsePerIMass = c.ContactNormal * impulse * dt;

    // Apply impulses: they are applied in the direction of the contact,
    // and in proportion to inverse mass.
    c.Object1->getComponent<RigidBody>()->setVelocity(c.Object1->getComponent<RigidBody>()->getVelocity() + impulsePerIMass * c.Object1->getComponent<RigidBody>()->getInvMass());
    // The other body goes in the opposite direction
    c.Object2->getComponent<RigidBody>()->setVelocity(c.Object2->getComponent<RigidBody>()->getVelocity() + impulsePerIMass * c.Object2->getComponent<RigidBody>()->getInvMass());
  }

  void ResolveVelocities(float dt, std::vector<Manifold> &contactlist)
  {
    unsigned iterationsRun = 0;
    unsigned maxIterations = contactlist.size() * 5;
    while (iterationsRun < maxIterations)
    {
      // Find the contact with the largest closing velocity;
      float maxVelocity = FLT_MAX;
      unsigned contactIndex = contactlist.size();
      for (unsigned i = 0; i < contactlist.size(); i++)
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
    float totalInverseMass = c.Object1->getComponent<RigidBody>()->getInvMass() + c.Object2->getComponent<RigidBody>()->getInvMass();

    // Find the amount of penetration resolution per unit of inverse mass
    glm::vec3 movePerIMass = c.ContactNormal * (c.Penetration / totalInverseMass) * dt;

    //If stack stability can be increased by not resolving all the penetrations
    //in one step
    movePerIMass *= 0.8f;//PHYSICS->PenetrationResolvePercentage;

    // Calculate the the movement amounts
    c.v1 = movePerIMass *  c.Object1->getComponent<RigidBody>()->getInvMass();// +c.v1;
    c.v2 = movePerIMass * -c.Object2->getComponent<RigidBody>()->getInvMass();// +c.v2;

    // Apply the penetration resolution

    c.Object1->getComponent<RigidBody>()->setPosition(c.Object1->getComponent<RigidBody>()->getPosition() + c.v1);
    c.Object2->getComponent<RigidBody>()->setPosition(c.Object2->getComponent<RigidBody>()->getPosition() + c.v2);
  }

  void ResolvePositions(float dt, std::vector<Manifold> &contactlist)
  {
    //Resolving penetrations is a tricky problem with large stacks. The only real way to get 
    //good results is by considering all contacts at once at solving them together. This method basically 
    //just keeps poking the bodies until the penetrations are below the penetration slop epsilon. 
    //Better solutions involve building contact graphs, shock propagation and constraint based
    //solvers.
    unsigned iterationsRun = 0;
    unsigned maxIterations = contactlist.size() * 5;
    const float positionEpsilon = 0.0001f;

    while (iterationsRun < maxIterations)
    {
      // Find biggest penetration greater than
      //the correction epsilon
      float maxPenetration = positionEpsilon;
      unsigned contactIndex = contactlist.size();
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

      // Update the penetrations for all related contacts
      glm::vec3 movement1 = contactlist[contactIndex].v1;
      glm::vec3 movement2 = contactlist[contactIndex].v2;

      for (unsigned i = 0; i < contactlist.size(); i++)
      {
        if (contactlist[i].Object1 == contactlist[contactIndex].Object1)
        {
          contactlist[i].Penetration -= glm::dot(movement1, contactlist[i].ContactNormal);
        }
        else if (contactlist[i].Object1 == contactlist[contactIndex].Object2)
        {
          contactlist[i].Penetration -= glm::dot(movement2, contactlist[i].ContactNormal);
        }

        if (contactlist[i].Object2)
        {
          if (contactlist[i].Object2 == contactlist[contactIndex].Object1)
          {
            contactlist[i].Penetration += glm::dot(movement1, contactlist[i].ContactNormal);
          }
          else if (contactlist[i].Object2 == contactlist[contactIndex].Object2)
          {
            contactlist[i].Penetration += glm::dot(movement2, contactlist[i].ContactNormal);
          }
        }
      }
      ++iterationsRun;
    }
  }

}
