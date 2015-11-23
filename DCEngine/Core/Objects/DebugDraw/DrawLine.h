/******************************************************************************/
/*!
@file	DrawLine.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

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