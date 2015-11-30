/******************************************************************************/
/*!
\file   System.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The base System class.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "System.h"
#include "../Engine/Engine.h"

DCEngine::System::~System()
{
  // Deregister from all publishers
  for (auto publisher : ActiveDelegateHolders) {
    //publisher->Dis
    Daisy->Disconnect<Entity>(publisher, this);
  }

}
