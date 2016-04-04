#pragma once

//#include"../../Components/PhysicsSpace.h"

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
    ZilchDeclareBaseType(Ray, Zilch::TypeCopyMode::ValueType);
    Vec3 Origin;
    Vec3 Direction;
  };

}