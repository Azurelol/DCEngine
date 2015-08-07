/*****************************************************************************/
/*!
\file   Level.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/5/2015
\brief  A level is a resource that stores a set of objects that can be loaded
        into a space.

*/
/******************************************************************************/
#pragma once

#include "Resource.h"

namespace DCEngine {

  class Level : public Resource {
  public:
    Level(std::string name) : Resource(name) {}
    ~Level();
  
  private:
    

  };

}

