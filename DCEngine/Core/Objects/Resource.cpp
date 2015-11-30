/*****************************************************************************/
/*!
\file   Resource.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/5/2015
\brief  Resources are the source data of a game. They are assets such as textures,
sounds, levels, scripts. They are the resources found in a library window.

Each resource is assigned an unique id when created, allowing objects
and archetypes to refer to resources and not break their connection
even if the name of the resource is changed.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.


*/
/******************************************************************************/

#include "Resource.h"

namespace DCEngine {

  Resource::Resource(std::string name) : Object(name) {
  }

  /**************************************************************************/
  /*!
  @brief  Removes the resource from the Content system, as well as deleting
          its serialized file from disk.
  @return The success of the operation.
  */
  /**************************************************************************/
  bool Resource::Destroy()
  {
    return false;
  }

}