#pragma once
#include "../DebugDraw.h"

namespace DCEngine {

  class DrawLineObj : public DebugDrawObject
  {
  public:
    Vec3 StartPos;
    Vec3 EndPos;
    Real Length;
    Vec4 Color;  
  };


}