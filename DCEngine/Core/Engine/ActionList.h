/*****************************************************************************/
/*!
\file   ActionList.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/6/2015
\brief  An action
Reference: http://gamedevelopment.tutsplus.com/tutorials/the-action-list-data-structure-good-for-ui-ai-animations-and-more--gamedev-9264
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once

#include <vector>

#include "Action.h"

namespace DCEngine {

  class ActionList {

  public:

  private:
    std::vector<Action> _actions;


  };

}