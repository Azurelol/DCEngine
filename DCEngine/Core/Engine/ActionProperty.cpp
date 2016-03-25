/*****************************************************************************/
/*!
@file      ActionProperty.cpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      3/24/2016
@brief     ActionProperty implementation.
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "ActionProperty.h"

// Engine
#include "Engine.h"

namespace DCEngine {
  
  ActionBaseProperty::ActionBaseProperty(ActionSetPtr set, Real duration, Ease ease)
    : Duration(duration), EaseType(ease)
  {
  }

  float ActionBaseProperty::Update(float dt)
  {
    return Interpolate(dt);
  }

}