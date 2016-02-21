/******************************************************************************/
/*!
@file   Module.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/20/2016
@brief  Modules are objects which can listen to and dispatch events.
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Module.h"

#include "../Engine/Engine.h"

namespace DCEngine {


  /**************************************************************************/
  /*!
  \brief  Module constructor.
  */
  /**************************************************************************/
  Module::Module(std::string name) : Object(name)
  {
  }

  /**************************************************************************/
  /*!
  \brief  Module destructor.
  */
  /**************************************************************************/
  Module::~Module()
  {
    // Deregister from all publishers
    for (auto publisher : ActiveDelegateHolders) {
      //publisher->Dis
      Daisy->Disconnect<Entity>(publisher, this);
    }
  }
}