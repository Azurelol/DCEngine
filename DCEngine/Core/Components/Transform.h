/******************************************************************************/
/*!
@file   Transform.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/05/2015
@brief  The transform component allows the representation of this object in
        world space, allowing it to be drawn, take part in collisions and
        force-based movement through the addition of other components.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {
  
  class GameObject;
  class Transform : public Component {
    friend class GameObject;
  public:
    
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


    Transform::Transform(Entity & owner) : Component(std::string("Transform"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

  private:
    
    Real3 Origin = Real3(0, 0, 0);
    void UpdateTranslation();
    void UpdateRotation();
    void UpdateScale();





  };

}