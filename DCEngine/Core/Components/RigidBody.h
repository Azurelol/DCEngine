#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  enum class DynamicStateType {
    Dynamic,
    Kinematic,
    Static
  };

  class RigidBody : public Component {
  public:
    Real3 Velocity = Real3(0, 0, 0);
    Real3 AngularVelocity = Real3(0, 0, 0);
    DynamicStateType DynamicState = DynamicStateType::Dynamic;
    bool AllowSleep = true;
    bool RotationLocked = false;
    float Mass;

    RigidBody(Entity& owner) : Component(std::string("RigidBody"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);
  };


}