/******************************************************************************/
/*!
@file   RigidBody.h
@author Blaine Reiner, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/24/2015
@brief  The RigidBody component handles calls the update of forces for the
object, such as acceleration, velocity, etc. This component interacts
directly with the physics system.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
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


  class Collision;
  class Resolution;

  namespace Components
  {
    class Transform;
    class RigidBody : public Component
    {
    public:
      friend class Collision;
      friend class Resolution;

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(RigidBody, Component);
#endif

      RigidBody(Entity& owner) : Component(std::string("RigidBody"), owner) {}
      ~RigidBody();
      void Initialize();
      void AddForce(Vec3 force);
      void Integrate(float dt);
      void PublishResults(void);

      // Properties
      /* It's possible that we want to use a mass-override component instead */
      
      DCE_DEFINE_PROPERTY(Vec3, AngularVelocity);
      DCE_DEFINE_PROPERTY(Real, Mass);
      DCE_DEFINE_PROPERTY(Boolean, RotationLocked);
      DCE_DEFINE_PROPERTY(DynamicStateType, DynamicState);

      Vec3 getVelocity(void);
      void setVelocity(Vec3 vel);

      /* @todo This should be private as the end-user will never ask for it. */
      float getInvMass(void);
      Vec3 getAcceleration(void);
      void setAcceleration(Vec3 accel);
      /* @todo The friction will be set by the 'physics' material the user sets.
                Why do a material instead of a value? This way we can point every object of
                a particular type at a physics material and set its friction coefficent/restituion
                in one place, and only have to update there. Otherwise whenever we wanted to change
                either we would have to change it for EACH object. */
      float getRestitution(void);
      void setRestitution(float rest);
      float getFriction(void);
      void setFriction(float friction);
      /* @todo Gravity-specific settings should be moved to a different
               component specifically for gravity. */
      Vec3 getGravityDir(void);
      Real getGravityRatio(void);
      bool getGravity(void);
      void setGravityDir(Vec3 dir);
      void setGravityRatio(Real ratio);
      void setGravity(bool state);


      void ApplyLinearVelocity(Vec3 vel);

    private:

      void setPosition(Vec3 pos);
      Vec3 getPosition(void);

      Transform* TransformComponent = NULL;; //!< Reference to the Transform component.

      /* Variables */
      DynamicStateType DynamicState = DynamicStateType::Dynamic;
      Vec3 InitialVelocity = Vec3(0, 0, 0);
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
      Real Friction = 0.5f;
      Boolean AllowSleep = true;
      Boolean RotationLocked = true;
      Real Damping = 0.999f; //!< How much the object's velocity is reduced arbitratrily. 


    };
  }


}