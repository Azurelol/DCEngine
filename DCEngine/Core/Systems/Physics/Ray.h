#pragma once

namespace DCEngine {

  /**************************************************************************/
  /*!
  @class A Ray is a linear arrangement of points that starts at a given location
  and extends infinitely far in a given direction. It is also known as a
  half-line.
  */
  /**************************************************************************/
  struct Ray {
    Vec3 Origin;
    Vec3 Direction;
  };

}