/******************************************************************************/
/*!
@file   RigidBody.h
@author Blaine Reiner, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/24/2015
@brief  The RigidBody component handles calls the update of forces for the
        object, such as acceleration, velocity, etc. This component interacts 
        directly with the physics system.
*/
/******************************************************************************/
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
		void PublishResults(void);

    float getRestitution(void);
    Real3 getVelocity(void);
    Real3 getAcceleration(void);
    float getFriction(void);
    float getMass(void);
    float getInvMass(void);
    Real3 getPosition(void);

    void setPosition(Real3 pos);
    void setVelocity(Real3 vel);
    void setRestitution(float rest);
    void setAcceleration(Real3 accel);
    void setFriction(float friction);
    void setMass(float mass);


	private:

		Transform* TransformComponent; //!< Reference to the Transform component.

									   /* Properties */
		DynamicStateType DynamicState = DynamicStateType::Dynamic;
		Real3 Velocity = Real3(0, 0, 0);
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