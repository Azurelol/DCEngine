/*****************************************************************************/
/*!
\file   Resource.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/5/2015
\brief  Resources are the source data of a game. They are assets such as textures,
        sounds, levels, scripts. They are the resources found in a library window.

        Each resource is assigned an unique id when created, allowing objects
        and archetypes to refer to resources and not break their connection
        even if the name of the resource is changed.

*/
/******************************************************************************/
#pragma once

#include "Object.h"

namespace DCEngine {

  class Resource : public Object {
  public:
    Resource(std::string name) : Object(name) {}
    std::string& FileLocation() { return _fileLocation; }

  protected:
    std::string _fileLocation;

  private:


  };

  using ResourcePtr = std::shared_ptr<Resource>;

}