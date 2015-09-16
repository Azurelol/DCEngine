#pragma once
#include "..\Component.h"

namespace DCEngine {
  
  class Transform : public Component {
  public:
    Transform::Transform(Entity & owner) : Component(std::string("Transform"),
                                             EnumeratedComponent::Transform,
                                             BitfieldComponent::Transform, owner) {}
    void Initialize();
    
    // Properties
    Real3 Translation;
    Real3 Rotation;
    Real3 Scale;

    Real3 WorldTranslation;
    Real3 WorldRotation;
    Real3 WorldScale;
    // When parented to an object
    Real3 LocalTranslation;
    Real3 LocalRotation;
    Real3 LocalScale;

  };

}