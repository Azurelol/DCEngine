/*****************************************************************************/
/*!
@file      ActionDelay.cpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      1/22/2016
@brief     An action...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Action.h"

#include "Engine.h"

namespace DCEngine {

  /*==============*
  *     Delay     *
  *==============*/
  /**************************************************************************/
  /*!
  @brief ActionDelay constructor.
  @param sequence A reference to the sequence of actions this action
  belongs to.
  @param dt The duration for this delay action.
  */
  /**************************************************************************/
  ActionDelay::ActionDelay(ActionSetPtr set, Real duration)
  {
    if (DCE_TRACE_ACTIONS_CTOR)
      DCTrace << "ActionDelay::ActionDelay: Constructed! \n";
    Duration = duration;
    IsBlocking = true;
  }

  /**************************************************************************/
  /*!
  @brief Updates this action.
  @param dt The time slice given.
  @return How much time was consumed during this action
  */
  /**************************************************************************/
  float ActionDelay::Update(float dt)
  {
    if (IsFinished)
      return 0.0f;

    auto timeLeft = Duration - Elapsed;
    Elapsed += dt;
    if (Elapsed >= Duration) {
      if (DCE_TRACE_ACTIONS_UPDATE)
        DCTrace << "ActionDelay::Update: Finished! \n";
      IsFinished = true;
    } else if (DCE_TRACE_ACTIONS_UPDATE)
      DCTrace << "ActionDelay::Update: dt = '" << dt << "', timeLeft = '" << timeLeft << "' \n";

    return std::max(0.0f, dt - timeLeft);
  }

}