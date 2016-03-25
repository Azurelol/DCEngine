/*****************************************************************************/
/*!
@file   Orientation.h
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
#pragma once
#include "ComponentReference.h"

namespace DCEngine {
  namespace Components
  {
    class Orientation : public Component {
    public:

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(Orientation, Component);
#endif

      Real AbsoluteAngle; //!< The angle of the object about the up vector.
      //Quaternion Rotation; //!< The rotation that this object defines. Useful to align the object's
                      // bases with the world bases (using the inverse rotation)
      Vec3 WorldForward = Vec3(0, 1, 0);
      Vec3 WorldUp = Vec3(0, 1, 0);
      Vec3 WorldRight = Vec3(0, 0, 1);
      //Quaternion WorldRotation;

      /* Look At */
      void LookAtPoint(Vec3 point);
      void LookAtPointWithUp(Vec3 point);
      void LookAtDirection(Vec3 direction);
      void LookAtDirectionWithUp(Vec3 direction);
      void LookAtUp(void);

      /* Get LookAt */
      Vec3 GetLookAtPointRotation(void);
      Vec3 GetLookAtDirectionRotation(void);
      Vec3 GetLookAtPointWithUpRotation(void);
      Vec3 GetLookAtDirectionWithUpRotation(void);

      float GetVectorAngle(Vec3 vector);

      /* Initialize */
      Orientation(Entity& owner) : Component(std::string("Orientation"), owner) {}
      void Initialize();

    private:
    };
  }

}