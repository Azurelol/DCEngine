#pragma once

#include"../../Components/PhysicsSpace.h"

namespace DCEngine {
  /**************************************************************************/
  /*!
  @class Ray A ray is a linear arrangement of points that starts at a given location
  and extends infinitely far in a given direction. It is also known as a
  half-line.
  */
  /**************************************************************************/
  struct Ray 
  {
#if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    ZilchDeclareBaseType(Ray, Zilch::TypeCopyMode::ValueType);
#endif
    Vec3 Origin;
    Vec3 Direction;
  };

}