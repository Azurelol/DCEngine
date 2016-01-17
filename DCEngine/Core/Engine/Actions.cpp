/*****************************************************************************/
/*!
@file      Actions.cpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      1/7/2016
@brief     An action...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Action.h"

namespace DCEngine {

  /*==============*
  *     Actions   *
  *==============*/
  /**************************************************************************/
  /*!
  @brief Creates an action sequence.
  @param owner A reference to the owner of this action sequence.
  */
  /**************************************************************************/
  ActionSetPtr Actions::Sequence(ActionSet & owner)
  {
    return ActionSetPtr();
  }

  /**************************************************************************/
  /*!
  @brief Creates an action sequence.
  @param owner A reference to the owner of this action sequence.
  */
  /**************************************************************************/
  ActionSetPtr Actions::Group(ActionSet & owner)
  {
    return ActionSetPtr();
  }

  /**************************************************************************/
  /*!
  @brief Creates an ActionCall.
  @param set A reference to the ActionSet that this action belongs to.
  @param fn A reference to the function that will be added.
  */
  /**************************************************************************/
  void Actions::Call(ActionSet & set, void* fn)
  {
    // Construct an action and add it to the action set
    
  }

  /**************************************************************************/
  /*!
  @brief Creates an ActionDelay
  @param set A reference to the ActionSet that this action belongs to.
  @param duration How long should the delay run for.
  */
  /**************************************************************************/
  void Actions::Delay(ActionSet & set, Real duration)
  {
    // Construct the action and add it to the action set
  }

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
  ActionCall::ActionCall(ActionSet & sequence, Delegate* funcPtr) : FunctionPtr(funcPtr)
  {

  }

  /**************************************************************************/
  /*!
  @brief Updates this action.
  @param dt The time slice given.
  */
  /**************************************************************************/
  float ActionCall::Update(float dt)
  {
    return 0.0f;
  }


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
  ActionDelay::ActionDelay(ActionSequence & sequence, Real duration)
  {
    Duration = duration;
    IsBlocking = true;
  }
  /**************************************************************************/
  /*!
  @brief Updates this action.
  @param dt The time slice given.
  @return How much time is remaining
  */
  /**************************************************************************/
  float ActionDelay::Update(float dt)
  {
    Elapsed += dt;
    if (Elapsed >= Duration)
      IsFinished = true;

    return std::max(0.0f, dt - Duration - Elapsed);
  }

}