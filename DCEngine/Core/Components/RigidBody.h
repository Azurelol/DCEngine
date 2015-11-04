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

		Vec3 InitialVelocity = Vec3(0, 0, 0);

		RigidBody(Entity& owner) : Component(std::string("RigidBody"), owner) {}

		void Initialize();
		virtual void Serialize(Json::Value& root);
		virtual void Deserialize(Json::Value& root);

		void AddForce(Vec3 force);
		void Integrate(float dt);
		void PublishResults(void);

		Vec3 getGravitydir(void);
		Real  getGravityratio(void);
		bool  getGravity(void);
		float getRestitution(void);
		Vec3 getVelocity(void);
		Vec3 getAcceleration(void);
		float getFriction(void);
		float getMass(void);
		float getInvMass(void);
		Vec3 getPosition(void);

		void setPosition(Vec3 pos);
		void setGravitydir(Vec3 dir);
		void setGravityratio(Real ratio);
		void setGravity(bool state);
		void setVelocity(Vec3 vel);
		void ApplyLinearVelocity(Vec3 vel);
		void setRestitution(float rest);
		void setAcceleration(Vec3 accel);
		void setFriction(float friction);
		void setMass(float mass);
		DynamicStateType DynamicState = DynamicStateType::Dynamic;

	private:

		Transform* TransformComponent; //!< Reference to the Transform component.

									   /* Properties */
		Real Gravityratio = 1.0f;
		bool Gravity = true;
		Vec3 Gravitydir = Vec3(0, -1, 0);
		Vec3 Velocity = Vec3(0, 0, 0);
		Vec3 AngularVelocity = Vec3(0, 0, 0);
		Vec3 Acceleration = Vec3(0, 0, 0);
		Vec3 Position = Vec3(0, 0, 0);
		Vec3 PreviousPosition = Vec3(0, 0, 0);
		Vec3 AccumulatedForce = Vec3(0, 0, 0);
		Real Mass = 10.0f;
		Real InverseMass = 1.0f / 10.0f;
		Real Restitution = 0.0f;
		Real Friction = 0.1f;
		Boolean AllowSleep = true;
		Boolean RotationLocked = true;
		Real Damping = 0.999f; //!< How much the object's velocity is reduced arbitratrily. 


	};


}