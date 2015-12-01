#include "Math.h""

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
    return Vec3(static_cast<int>(val.x) + snapDistance,
                static_cast<int>(val.y) + snapDistance,
                static_cast<int>(val.z) + snapDistance);
  }
  std::ostream& operator<<(std::ostream & os, Vec3 & const vec)
  {
    os << "Vec3 - x: '" << vec.x << "', y: '" << vec.y << "', z: '" << vec.z << "' \n";
    return os;
  }
}