#include "CollisionGroup.h"
#include "../../Core/Engine/Engine.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief  Returns the texture resource to be used by the graphics system.
  @return A reference to the texture object.
  */
  /**************************************************************************/
  CollisionGroupPtr CollisionGroup::Find(std::string name)
  {
    return Daisy->getSystem<Systems::Content>()->getCollisionGroup(name);
  }

  CollisionGroup & CollisionGroup::operator=(const CollisionGroup & rhs)
  {
    this->ObjectName = rhs.ObjectName;
  }
}