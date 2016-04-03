/******************************************************************************/
/*!
@file   RigidBody.cpp
@author Blaine Reiner, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/24/2015
@brief  The RigidBody component handles calls the update of forces for the
object, such as acceleration, velocity, etc. This component interacts
directly with the physics system.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "RigidBody.h"
#include "PhysicsSpace.h"
#include "EngineReference.h"

namespace DCEngine
{
  namespace Components
  {
    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(RigidBody, "RigidBody", DCEngineCore, builder, type) {
      // Constructor / Destructor
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(RigidBody);
      // Fields
      ZilchBindField(builder, type, &RigidBody::Mass, "Mass", Zilch::PropertyBinding::Get);
      // Properties
      DCE_BINDING_DEFINE_PROPERTY(RigidBody, Velocity);
      DCE_BINDING_DEFINE_PROPERTY(RigidBody, AngularVelocity);
      DCE_BINDING_DEFINE_PROPERTY(RigidBody, Mass);
      DCE_BINDING_DEFINE_PROPERTY(RigidBody, RotationLocked);
      ZilchBindMethod(builder, type, &RigidBody::ApplyLinearVelocity, ZilchNoOverload, "ApplyLinearVelocity", "vel");
      ZilchBindMethod(builder, type, &RigidBody::ApplyForce, ZilchNoOverload, "ApplyForce", "force");
      
    }
    #endif

    unsigned RigidBody::Created = 0;
    unsigned RigidBody::Destroyed = 0;
    unsigned RigidBody::Active = 0;

    /**************************************************************************/
    /*!
    @brief RigidBody constructor
    */
    /**************************************************************************/
    RigidBody::RigidBody(Entity & owner) : Component(std::string("RigidBody"), owner)
    {
      Created++;
      Active++;
    }

    /**************************************************************************/
    /*!
    @brief RigidBody destructor.
    */
    /**************************************************************************/
    RigidBody::~RigidBody()
    {
      Destroyed++;
      Active--;
      SpaceRef->getComponent<PhysicsSpace>()->RemoveRigidBody(*this);
    }

    /**************************************************************************/
    /*!
    @brief Initializes this component.
    */
    /**************************************************************************/
    void RigidBody::Initialize()
    {
      auto owner = dynamic_cast<GameObject*>(Owner());
      // Store a reference to the Transform Component
      TransformComponent = owner->getComponent<Transform>();
      // Subscribe this physics component to the physics space
      SpaceRef->getComponent<PhysicsSpace>()->AddRigidBody(*this);
      // Stores the initial position
      Position = TransformComponent->Translation;
    }

    /**************************************************************************/
    /*!
    @brief Adds a force on to this rigidbody.
    @param A force vector.
    */
    /**************************************************************************/
    void RigidBody::ApplyForce(Vec3 force)
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
      Vec3 newAcceleration;

      if (Gravity)
      {
        newAcceleration = AccumulatedForce * InverseMass + Acceleration + (Gravitydir * (PHYSICS::Gravity * Gravityratio));
      }
      else
      {
        newAcceleration = AccumulatedForce * InverseMass + Acceleration;
      }

      // Integrate the velocity
      Velocity = Velocity + newAcceleration * dt;

      // Dampen the velocity for numerical stability and soft drag
      Velocity *= std::pow(Damping, dt);

      // Clamp to velocity max for numerical stability
      if (glm::dot(Velocity, Velocity) > 10 * 1000.0f)
      {
        Velocity = glm::normalize(Velocity);
        Velocity = Velocity * 100.0f;
      }

      // Clear the force
      AccumulatedForce = Vec3(0, 0, 0);
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


    /**************************************************************************/
    /**************************************************************************!
                                     PROPERTIES

    /**************************************************************************/

    /**************************************************************************/
    /*!
    \brief Updates this object's translation with the calculated position.
    */
    /**************************************************************************/
    void RigidBody::setPosition(Vec3 pos)
    {
      Position = pos;
    }


    /**************************************************************************/
    /*!
    \brief Updates this object's direction of gravity.
    */
    /**************************************************************************/
    void RigidBody::setGravityDir(Vec3 dir)
    {
      Gravitydir = glm::normalize(dir);
    }

    /**************************************************************************/
    /*!
    \brief Updates this object's ratio of gravity.
    */
    /**************************************************************************/
    void RigidBody::setGravityRatio(Real ratio)
    {
      Gravityratio = ratio;
    }


    /**************************************************************************/
    /*!
    \brief Updates if this object applies gravity gravity.
    */
    /**************************************************************************/
    void RigidBody::setGravity(bool state)
    {
      Gravity = state;
    }


    /**************************************************************************/
    /*!
    \brief Updates this object's velocity.
    */
    /**************************************************************************/
    void RigidBody::setVelocity(Vec3 vel)
    {
      Velocity = vel;
    }
    
    /**************************************************************************/
    /*!
    \brief Updates this object's velocity based on current velocity.
    */
    /**************************************************************************/
    void RigidBody::ApplyLinearVelocity(Vec3 vel)
    {
      Velocity += vel;
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
    void RigidBody::setAcceleration(Vec3 accel)
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
    \brief gets this object's Restitution.
    */
    /**************************************************************************/
    float RigidBody::getRestitution(void)
    {
      return this->Restitution;
    }

    Vec3 RigidBody::getPosition(void)
    {
      return this->Position;
    }

    /**************************************************************************/
    /*!
    \brief gets this object's Velocity.
    */
    /**************************************************************************/
    Vec3 RigidBody::getVelocity(void)
    {
      if (DynamicState == DynamicStateType::Static)
      {
        return Vec3(0, 0, 0);
      }

      return this->Velocity;
    }

    /**************************************************************************/
    /*!
    \brief gets this object's Acceleration.
    */
    /**************************************************************************/
    Vec3 RigidBody::getAcceleration(void)
    {
      if (DynamicState == DynamicStateType::Static)
      {
        return Vec3(0, 0, 0);
      }

      if (Gravity)
      {
        return (Acceleration + (Gravitydir * (Gravityratio * PHYSICS::Gravity)));
      }

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
    \brief gets this object's Inverse Mass.
    */
    /**************************************************************************/
    float RigidBody::getInvMass(void)
    {
      return InverseMass;
    }

    /**************************************************************************/
    /*!
    \brief gets this object's direction of gravity.
    */
    /**************************************************************************/
    Vec3 RigidBody::getGravityDir(void)
    {
      return Gravitydir;
    }

    /**************************************************************************/
    /*!
    \brief gets this object's ratio of gravity.
    */
    /**************************************************************************/
    Real RigidBody::getGravityRatio(void)
    {
      return Gravityratio;
    }

    /**************************************************************************/
    /*!
    \brief gets this object's state of gravity.
    */
    /**************************************************************************/
    bool RigidBody::getGravity(void)
    {
      return Gravity;
    }

  }

}
