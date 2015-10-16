#include "RigidBody.h"
#include "PhysicsSpace.h"
#include "EngineReference.h"

namespace DCEngine
{

  /**************************************************************************/
  /*!
  @brief Initializes this component.
  @note  
  */
  /**************************************************************************/
	void RigidBody::Initialize()
	{
		auto owner = dynamic_cast<GameObject*>(Owner());
		// Store a reference to the Transform Component
		TransformComponent = owner->getComponent<Transform>();
		// Subscribe this physics component to the physics space
		SpaceRef->getComponent<PhysicsSpace>()->AddRigidBody(owner);
    // Stores the initial position
		Position = TransformComponent->Translation;
	}

  /**************************************************************************/
  /*!
  @brief Serializes this component.
  @param 
  @note
  */
  /**************************************************************************/
	void RigidBody::Serialize(Json::Value & root)
	{

	}

  /**************************************************************************/
  /*!
  @brief Deserializes this component.
  @param 
  @note
  */
  /**************************************************************************/
	void RigidBody::Deserialize(Json::Value & root)
	{

	}

	/**************************************************************************/
	/*!
	@brief Adds a force on to this rigidbody.
  @param A force vector.
	*/
	/**************************************************************************/
	void RigidBody::AddForce(Real3 force)
	{
		AccumulatedForce += force;
	}


	/**************************************************************************/
	/*!
	@brief Update acceleration, velocity, position and other properties.
  @param The delta time.
  @note  
	*/
	/**************************************************************************/
	void RigidBody::Integrate(float dt)
	{
		// Do not integrate static bodies
		if (DynamicState == DynamicStateType::Static)
		{
			return;
		}

		Position = TransformComponent->Translation;

		// Store prev position
		PreviousPosition = Position;

		// Integrate the position using Euler 
		Position = Position + Velocity * dt; //acceleration term is small

											 //Determine the acceleration
											 //Acceleration = PHYSICS->Gravity; put in check for gravity component or something
		Real3 newAcceleration = AccumulatedForce * InverseMass + Acceleration;

		// Integrate the velocity
		Velocity = Velocity + newAcceleration * dt;

		// Dampen the velocity for numerical stability and soft drag
		Velocity *= std::pow(Damping, dt);

		// Clamp to velocity max for numerical stability

		if (glm::dot(Velocity, Velocity) > 144.0f/*PHYSICS->MaxVelocitySq*/)
		{
			glm::normalize(Velocity);
			Velocity = Velocity * 12.0f * dt/*PHYSICS->MaxVelocity*/;
		}

		// Clear the force
		AccumulatedForce = Real3(0, 0, 0);
	}

	/**************************************************************************/
	/*!
	\brief Updates this object's translation with the calculated position.
	*/
	/**************************************************************************/
	void RigidBody::setPosition(Real3 pos)
	{
		Position = pos;
	}


	/**************************************************************************/
	/*!
	\brief Updates this object's velocity.
	*/
	/**************************************************************************/
	void RigidBody::setVelocity(Real3 vel)
	{
		Velocity = vel;
	}


  /**************************************************************************/
  /*!
  \brief Updates this object's restitution.
  */
  /**************************************************************************/
  void RigidBody::setRestitution(float rest)
  {
    Restitution = rest;
  }

  /**************************************************************************/
  /*!
  \brief Updates this object's acceleration.
  */
  /**************************************************************************/
  void RigidBody::setAcceleration(Real3 accel)
  {
    Acceleration = accel;
  }

  /**************************************************************************/
  /*!
  \brief Updates this object's friction.
  */
  /**************************************************************************/
  void RigidBody::setFriction(float friction)
  {
    Friction = friction;
  }

  /**************************************************************************/
  /*!
  \brief Updates this object's mass.
  */
  /**************************************************************************/
  void RigidBody::setMass(float mass)
  {

  }


  /**************************************************************************/
  /*!
  \brief gets this object's Restitution.
  */
  /**************************************************************************/
  float RigidBody::getRestitution(void)
  {
    return Restitution;
  }

  Real3 RigidBody::getPosition(void)
  {
    return Position;
  }

  /**************************************************************************/
  /*!
  \brief gets this object's Velocity.
  */
  /**************************************************************************/
  Real3 RigidBody::getVelocity(void)
  {
    return Velocity;
  }

  /**************************************************************************/
  /*!
  \brief gets this object's Acceleration.
  */
  /**************************************************************************/
  Real3 RigidBody::getAcceleration(void)
  {
    return Acceleration;
  }

  /**************************************************************************/
  /*!
  \brief gets this object's Friction.
  */
  /**************************************************************************/
  float RigidBody::getFriction(void)
  {
    return Friction;
  }

  /**************************************************************************/
  /*!
  \brief gets this object's Mass.
  */
  /**************************************************************************/
  float RigidBody::getMass(void)
  {
    return Mass;
  }

  float RigidBody::getInvMass(void)
  {
    return InverseMass;
  }

	/**************************************************************************/
	/*!
	\brief actually push the position to what it should be.
	*/
	/**************************************************************************/
	void RigidBody::PublishResults(void)
	{
		TransformComponent->Translation = Position;
	}


}