/*****************************************************************************/
/*!
@file      ActionCall.cpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      1/22/2016
@brief     An action...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Engine.h"

namespace DCEngine {

  /*==============*
  *     Call     *
  *==============*/
  /**************************************************************************/
  /*!
  @brief Action Call constructor.
  @param sequence A reference to the sequence that this action will be added to.
  @param funcPtr A pointer to the function that will need to be called.
  */
  /**************************************************************************/
  ActionCall::ActionCall(ActionSetPtr set, Delegate* funcPtr) : FunctionPtr(funcPtr)
  {

  }

  /**************************************************************************/
  /*!
  @brief Updates this action.
  @param dt The time slice given.
  @return How muc time was consumed.
  */
  /**************************************************************************/
  float ActionCall::Update(float dt)
  {
    // Call the functiom immediately

    // No time was consumed
    return 0.0f;
  }

}