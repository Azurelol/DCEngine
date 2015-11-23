/******************************************************************************/
/*!
@file   DebugDraw.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "Object.h"

namespace DCEngine {

  class DebugDrawObject : public Object {

  public:    
    virtual void Draw();

  private:

  };

  using DebugDrawObjPtr = std::shared_ptr<DebugDrawObject>;
  using DebugDrawObjVec = std::vector<DebugDrawObjPtr>;

}