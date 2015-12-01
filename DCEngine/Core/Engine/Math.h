#pragma once


namespace DCEngine {

  class Math {
  public:
    static void PrintVec3(Vec3&);
    static Vec2 Snap(Vec2&, float snapDistance = 1.0f);
    static Vec3 Snap(Vec3&, float snapDistance = 1.0f);

    friend std::ostream& operator<<(std::ostream& os, Vec3& const vec);

  private:
    


  };

}
