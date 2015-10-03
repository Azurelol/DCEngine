#include "RigidBody.h"
#include "PhysicsSpace.h"
#include "EngineReference.h"

namespace DCEngine
{

	void RigidBody::Initialize()
	{
		auto owner = dynamic_cast<GameObject*>(Owner());
		// Store a reference to the Transform Component
		TransformComponent = owner->getComponent<Transform>();
		// Subscribe this physics component to the physics space
		space_->getComponent<PhysicsSpace>()->AddRigidBody(owner);

		Position = TransformComponent->Translation;
	}

	void RigidBody::Serialize(Json::Value & root)
	{

	}

	void RigidBody::Deserialize(Json::Value & root)
	{

	}


	void RigidBody::AddForce(Real3 force)
	{
		AccumulatedForce += force;
	}

	void RigidBody::Integrate(float dt)
	{
		//Do not integrate static bodies
		if (DynamicState == DynamicStateType::Static)
		{
			return;
		}

		Position = TransformComponent->Translation;

		//Store prev position
		PreviousPosition = Position;

		//Integrate the position using Euler 
		Position = Position + Velocity * dt; //acceleration term is small

											 //Determine the acceleration
											 //Acceleration = PHYSICS->Gravity; put in check for gravity component or something
		Real3 newAcceleration = AccumulatedForce * InverseMass + Acceleration;

		//Integrate the velocity
		Velocity = Velocity + newAcceleration * dt;

		//Dampen the velocity for numerical stability and soft drag
		Velocity *= std::pow(Damping, dt);

		//Clamp to velocity max for numerical stability

		if (glm::dot(Velocity, Velocity) > 144.0f/*PHYSICS->MaxVelocitySq*/)
		{
			glm::normalize(Velocity);
			Velocity = Velocity * 12.0f * dt/*PHYSICS->MaxVelocity*/;
		}

		//Clear the force
		AccumulatedForce = Real3(0, 0, 0);
	}

	void RigidBody::SetPosition(Real3 pos)
	{
		TransformComponent->Translation = pos;
	}

	void RigidBody::SetVelocity(Real3 vel)
	{
		Velocity = vel;
	}

	void RigidBody::PublishResults(void)
	{
		TransformComponent->Translation = Position;
	}


}