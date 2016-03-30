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
  ActionCall::ActionCall(ActionSetPtr set, Delegate* funcPtr) : Action("ActionCall"), FunctionPtr(funcPtr)
  {
    if (DCE_TRACE_ACTIONS_CTOR)
      DCTrace << "ActionCall::ActionCall: Constructed! \n";
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
    if (DCE_TRACE_ACTIONS_UPDATE)
      DCTrace << "ActionCall::Update: dt = '" << dt << "'\n";
    // Call the functiom immediately
    FunctionPtr->Call();
    // No time was consumed
    if (DCE_TRACE_ACTIONS_UPDATE)
      DCTrace << "ActionCall::Update: Finished! \n";
    this->IsFinished = true;
    return 0.0f;

  }

}