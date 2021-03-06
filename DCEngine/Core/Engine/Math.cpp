/*****************************************************************************/
/*!
@file   Math.cpp
@author Christian Sagel, Blaine Reiner
@par    email: c.sagel\@digipen.edu , blaine.reiner\@digipen.edu
@date   10/01/2015
@note   This file includes all the headers for the components that Rebound
uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Math.h"

namespace DCEngine {
  
  void Math::PrintVec3(Vec3 &)
  {

  }

  Vec2 Math::Snap(Vec2 & val, float snapDistance)
  {
    Vec2 Result;

    float remainderx = Modulus(val.x, abs(snapDistance));
    float remaindery = Modulus(val.y, abs(snapDistance));

    if ((snapDistance - remainderx) > remainderx)
    {
      Result.x = val.x - remainderx;
    }
    else
    {
      Result.x = (val.x - remainderx) + snapDistance;
    }

    if ((snapDistance - remaindery) > remaindery)
    {
      Result.y = val.y - remaindery;
    }
    else
    {
      Result.y = (val.y - remaindery) + snapDistance;
    }
    
    return Result;
  }

  float Math::Modulus(float val, float divisor)
  {
    for (; val > divisor; val -= divisor)
    {}

    for (; val < -divisor; val += divisor)
    {}

    return val;
  }

  Vec3 Math::Snap(Vec3 & val, float snapDistance)
  {
    Vec3 Result;

    float remainderx = Modulus(val.x, abs(snapDistance));
    float remaindery = Modulus(val.y, abs(snapDistance));
    float remainderz = Modulus(val.z, abs(snapDistance));

    if ((snapDistance - remainderx) > remainderx)
    {
      Result.x = val.x - remainderx;
    }
    else
    {
      Result.x = (val.x - remainderx) + snapDistance;
    }

    if ((snapDistance - remaindery) > remaindery)
    {
      Result.y = val.y - remaindery;
    }
    else
    {
      Result.y = (val.y - remaindery) + snapDistance;
    }

    if ((snapDistance - remainderz) > remainderz)
    {
      Result.z = val.z - remainderz;
    }
    else
    {
      Result.z = (val.z - remainderz) + snapDistance;
    }

    return Result;
  }

  std::ostream& operator<<(std::ostream & os, const Vec3 & vec)
  {
    os << "Vec3 - x: '" << vec.x << "', y: '" << vec.y << "', z: '" << vec.z << "' \n";
    return os;
  }

  // interpolation funtions from here on //

   Vec3 Math::lerp(Vec3 start, Vec3 end, float ratio)
  {
    return start + (end - start) * ratio;
  }

   Vec2 Math::lerp(Vec2 start, Vec2 end, float ratio)
  {
    return start + (end - start) * ratio;
  }
   float Math::lerp(float start, float end, float ratio)
  {
    return start + (end - start) * ratio;
  }

  Vec3  Math::interpsin(Vec3 start, Vec3 end, float ratio)
  {
    return start + (end - start) * sin(ratio);
  }
  Vec2  Math::interpsin(Vec2 start, Vec2 end, float ratio)
  {
    return start + (end - start) * sin(ratio);
  }
  float Math::interpsin(float start, float end, float ratio)
  {
    return start + (end - start) * sin(ratio);
  }

  Vec3  Math::interpcos(Vec3 start, Vec3 end, float ratio)
  {
    return start + (end - start) * cos(ratio);
  }
  Vec2  Math::interpcos(Vec2 start, Vec2 end, float ratio)
  {
    return start + (end - start) * cos(ratio);
  }
  float Math::interpcos(float start, float end, float ratio)
  {
    return start + (end - start) * cos(ratio);
  }

  void Math::ClampString(const char * input, char * output, int length)
  {
    for (int i = 0; i < length; ++i)
    {
      output[i] = input[i];
    }

    output[length] = 0;
  }
}