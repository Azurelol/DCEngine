/*****************************************************************************/
/*!
\file   CollisionGroup.h
\author Blaine, Christian Sagel
\par    email: c.sagel\@digipen.edu, blaine.reiner\@digipen.edu
\date   11/19/2015
\brief  Collision groups are used as a tag to identify an object in the table.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"

namespace DCEngine {

  class CollisionGroup : public Resource {
  public:
    CollisionGroup(std::string name) : Resource(name) {}
    ~CollisionGroup() {}

  private:

  };

  using CollisionGroupHandle = std::string;

}