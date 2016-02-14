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
    static Vec3  lerp(Vec3  start, Vec3  end, float ratio);
    static Vec2  lerp(Vec2  start, Vec2  end, float ratio);
    static float lerp(float start, float end, float ratio);
    static Vec3  interpsin(Vec3 start, Vec3 end, float ratio);
    static Vec2  interpsin(Vec2 start, Vec2 end, float ratio);
    static float interpsin(float start, float end, float ratio);
    static Vec3  interpcos(Vec3 start, Vec3 end, float ratio);
    static Vec2  interpcos(Vec2 start, Vec2 end, float ratio);
    static float interpcos(float start, float end, float ratio);
    static void ClampString(const char * input, char * output, int length);
    friend std::ostream& operator<<(std::ostream& os, const Vec3& vec);

  private:
    


  };

}
