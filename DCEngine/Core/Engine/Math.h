/*****************************************************************************/
/*!
@file   Math.h
@author Christian Sagel, Blaine Reiner
@par    email: c.sagel\@digipen.edu , blaine.reiner\@digipen.edu
@date   10/01/2015
@note   This file includes all the headers for the components that Rebound
uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once


namespace DCEngine {

  class Math {
  public:
    static void PrintVec3(Vec3&);
    static Vec2 Snap(Vec2&, float snapDistance = 0.5f);
    static Vec3 Snap(Vec3&, float snapDistance = 0.5f);
    static float Modulus(float val, float divisor);

    friend std::ostream& operator<<(std::ostream& os, const Vec3& vec);

  private:
    


  };

}
