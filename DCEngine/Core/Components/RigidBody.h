#pragma once
#include "ComponentReference.h"

namespace DCEngine
{

	enum class DynamicStateType
	{
		Dynamic,
		Kinematic,
		Static
	};

	class Transform;
	class RigidBody : public Component
	{
	public:

		Real3 InitialVelocity = Real3(0, 0, 0);


		RigidBody(Entity& owner) : Component(std::string("RigidBody"), owner) {}

		void Initialize();
		virtual void Serialize(Json::Value& root);
		virtual void Deserialize(Json::Value& root);

		void AddForce(Real3 force);
		void Integrate(float dt);
		void SetPosition(Real3 pos);
		void SetVelocity(Real3 vel);
		void PublishResults(void);


	private:

		Transform* TransformComponent; //!< Reference to the Transform component.

									   /* Properties */
		DynamicStateType DynamicState = DynamicStateType::Dynamic;
		Real3 Velocity = Real3(5, 0, 0);
		Real3 AngularVelocity = Real3(0, 0, 0);
		Real3 Acceleration = Real3(0, 0, 0);
		Real3 Position = Real3(0, 0, 0);
		Real3 PreviousPosition = Real3(0, 0, 0);
		Real3 AccumulatedForce = Real3(0, 0, 0);
		Real Mass = 10.0f;
		Real InverseMass = 1.0f / 10.0f;
		Real Restitution = 1.0f;
		Real Friction = 0.5;
		Boolean AllowSleep = true;
		Boolean RotationLocked = true;
		Real Damping = 0.9999f; //!< How much the object's velocity is reduced arbitratrily. 


	};


}