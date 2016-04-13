/******************************************************************************/
/*!
@file   Transform.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/05/2015
@brief  The transform component allows the representation of this object in
        world space, allowing it to be drawn, take part in collisions and
        force-based movement through the addition of other components.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"


namespace DCEngine {
 
  namespace
  {
    float PI = 3.14159265359f;
  }

  class GameObject;
  namespace Components
  {
    class Transform : public Component {
      friend class GameObject;
    public:

      // Properties
      Vec3 Translation;
      Vec3 Rotation;
      Vec3 Scale;

      const Vec3 & getLocalTranslation() const;
      void setLocalTranslation(const Vec3 & val);
      const Vec3 & getTranslation() const;
      void setTranslation(const Vec3 & val);
      const Vec3 & getWorldTranslation() const;
      void setWorldTranslation(const Vec3 & val);
      void SetLocalTranslation(const Vec3 & pos);
      DCE_DEFINE_PROPERTY(Vec3, Rotation);
      DCE_DEFINE_PROPERTY(Vec3, Scale);
      // Relative to the world
      Vec3 WorldTranslation = Translation;
      Vec3 WorldRotation = Rotation;
      Vec3 WorldScale = Scale;
      // Relative to self
      Vec3 LocalTranslation = Translation;
      Vec3 LocalRotation = Rotation;
      Vec3 LocalScale = Scale;
      

      //const Vec3 &getRotation(void);
      //void setRotation(Vec3 const & rot);
      TransformDataPair getTransformDataPair();
      ZilchDeclareDerivedType(Transform, Component);
      Transform::Transform(Entity & owner);
      void Initialize();
      void UpdateTranslation();
      void UpdateRotation();
      Vec3 RotatePoint(Vec3 point, Vec3 rotation, float angle);

    private:

      Vec3 DRotation;
      Vec3 Origin = Vec3(0, 0, 0);
      Vec3 PrevTranslation = Translation;
      Vec3 PrevRotation = Rotation;
      bool firstloop = true;
      void UpdateScale();
    };
  }




}