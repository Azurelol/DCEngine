#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  enum class DynamicStateType {
    Dynamic,
    Kinematic,
    Static
  };

  class Transform;
  class RigidBody : public Component {
  public:
    
    Real3 InitialVelocity = Real3(0, 0, 0);    
    

    RigidBody(Entity& owner) : Component(std::string("RigidBody"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);
    void AddForce();

  private:

    Transform* TransformComponent; //!< Reference to the Transform component.
    
    /* Properties */
    DynamicStateType DynamicState = DynamicStateType::Dynamic;
    Real3 Velocity = Real3(0, 0, 0);
    Real3 AngularVelocity = Real3(0, 0, 0);
    Real Acceleration = 0;
    Real3 Position = Real3(0, 0, 0);
    Real3 PreviousPosition = Real3(0, 0, 0);
    Real Mass;
    Real InverseMass;
    Real Restitution;
    Real Friction;
    Boolean AllowSleep = true;
    Boolean RotationLocked = false;
    Real Damping; //!< How often the object's velocity is reduced arbitratrily. 


  };


}