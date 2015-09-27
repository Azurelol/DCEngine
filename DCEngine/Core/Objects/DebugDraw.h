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