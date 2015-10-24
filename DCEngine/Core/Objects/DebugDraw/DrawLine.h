#pragma once
#include "../DebugDraw.h"

namespace DCEngine {

  class DrawLineObj : public DebugDrawObject
  {
  public:
    Real3 StartPos;
    Real3 EndPos;
    Real Length;
    Real4 Color;  
  };


}