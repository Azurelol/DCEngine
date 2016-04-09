/*****************************************************************************/
/*!
@file   Orientation.cpp
@author Blaine Reiner, Christian Sagel
@par    email: blaine.reiner@digipen.edu, c.sagel\@digipen.edu
@date   10/13/2015
@brief  The orientation component is an utility component that helps make the
manipulation of the transform of an object easier. It can be particularly
useful when the object has its own sense of forward. By using the
orientation component, the object can then be easily manipulated
relatively to the object's forward instead of being restricted to the
world axis.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Orientation.h"
#include "EngineReference.h"

namespace DCEngine 
{
  namespace Components
  {
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(Orientation, "Orientation", DCEngineCore, builder, type) {
      // Constructor / Destructor
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Orientation);
    }
    #endif


    void Orientation::Initialize()
    {
      return;
    }

    float Orientation::GetVectorAngle(Vec3 vector)
    {
      return std::atan2f(vector.x, -vector.y);
    }

    void Orientation::LookAtPoint(Vec3 point)
    {
      Vec3 Translation = this->Owner()->getComponent<Transform>()->Translation;

      Vec3 temp = glm::normalize(point - Translation);

      LookAtDirection(temp);
    }

    void Orientation::LookAtPointWithUp(Vec3 point)
    {
      Vec3 Translation = this->Owner()->getComponent<Components::Transform>()->Translation;

      Vec3 temp = glm::normalize(point - Translation);

      this->Owner()->getComponent<Components::Transform>()->Rotation.z = glm::acos(temp.x) + glm::half_pi<float>();
    }

    void Orientation::LookAtDirection(Vec3 direction)
    {
      direction = glm::normalize(direction);

      float NextRot = GetVectorAngle(direction);

      float CurrentRot = GetVectorAngle(WorldForward);

      float DeltaRot = CurrentRot - NextRot;

      CurrentRot -= DeltaRot;

      // convert to degrees first

      DeltaRot = static_cast<float>(DeltaRot * 180.0f / 3.14159265359);

      this->Owner()->getComponent<Components::Transform>()->Rotation.z -= DeltaRot;

      WorldForward = direction;
    }

    void Orientation::LookAtDirectionWithUp(Vec3 direction)
    {

    }

    void Orientation::LookAtUp(void)
    {
      LookAtDirection(WorldUp);
    }



    Vec3 Orientation::GetLookAtPointRotation(void)
    {
      return Vec3(0, 0, 0);
    }

    Vec3 Orientation::GetLookAtDirectionRotation(void)
    {
      return Vec3(0, 0, 0);
    }
    Vec3 Orientation::GetLookAtPointWithUpRotation(void)
    {
      return Vec3(0, 0, 0);
    }

    Vec3 Orientation::GetLookAtDirectionWithUpRotation(void)
    {
      return Vec3(0, 0, 0);
    }



  }

}