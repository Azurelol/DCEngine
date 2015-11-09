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
    
    #if (DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDeclareDerivedType(Transform, Component);
    #endif

    // Properties

    /* Properties*/
    Vec3 getTranslation();
    void setTranslation(Vec3);
    Vec3 getRotation();
    void setRotation(Vec3);
    Vec3 getScale();
    void setScale(Vec3);

    Vec3 Translation = Vec3(0.0f, 0.0f, 0.0f);
    Vec3 Rotation = Vec3(0.0f, 0.0f, 0.0f);
    Vec3 Scale = Vec3(1.0f, 1.0f, 1.0f);
    // Relative to the world
    Vec3 WorldTranslation = Translation;
    Vec3 WorldRotation = Rotation;
    Vec3 WorldScale = Scale;
    // Relative to self
    Vec3 LocalTranslation = Translation;
    Vec3 LocalRotation = Rotation;
    Vec3 LocalScale = Scale;   

    Transform::Transform(Entity & owner) : Component(std::string("Transform"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

  private:
    
    Vec3 Origin = Vec3(0, 0, 0);
    void UpdateTranslation();
    void UpdateRotation();
    void UpdateScale();





  };

}