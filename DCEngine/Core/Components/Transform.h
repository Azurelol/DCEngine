#pragma once
#include "ComponentReference.h"

namespace DCEngine {
  
  class Transform : public Component {
  public:
    Transform::Transform(Entity & owner) : Component(std::string("Transform"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);
    
    // Properties
    Real3 Translation = Real3(0.0f, 0.0f, 0.0f);
    Real3 Rotation = Real3(0.0f, 0.0f, 0.0f);
    Real3 Scale = Real3(1.0f, 1.0f, 1.0f);
    // Relative to the world
    Real3 WorldTranslation = Translation;
    Real3 WorldRotation = Rotation;
    Real3 WorldScale = Scale;
    // Relative to self
    Real3 LocalTranslation = Translation;
    Real3 LocalRotation = Rotation;
    Real3 LocalScale = Scale;

  };

}