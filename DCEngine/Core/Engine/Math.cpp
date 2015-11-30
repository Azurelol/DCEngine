#include "Math.h""

namespace DCEngine {
  
  void Math::PrintVec3(Vec3 &)
  {

  }

  Vec2 Math::Snap(Vec2 & val, float snapDistance)
  {
    return Vec2( static_cast<int>(val.x) + snapDistance,
                 static_cast<int>(val.y) + snapDistance);
                  
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