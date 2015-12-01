/*****************************************************************************/
/*!
@file   CollisionGroup.cpp
@author Blaine Reiner
@par    email: blaine.reiner\@digipen.edu
@date   10/01/2015
@note   This file includes all the headers for the components that Rebound
uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
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

    return *this;
  }
}