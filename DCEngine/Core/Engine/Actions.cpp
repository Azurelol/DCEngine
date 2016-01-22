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

#include "Engine.h"

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
    // Construct an action group
    ActionSetPtr sequence(new ActionSequence());
    // Add this action group to the set
    owner.Add(sequence);
    // Return a reference to this action group
    return sequence;
  }

  /**************************************************************************/
  /*!
  @brief Creates an action sequence.
  @param owner A reference to the owner of this action sequence.
  */
  /**************************************************************************/
  ActionSetPtr Actions::Group(ActionSet & owner)
  {    
    // Construct an action group
    ActionSetPtr group(new ActionGroup());
    // Add this action group to the set
    owner.Add(group);
    // Return a reference to this action group
    return group;
  }

  /**************************************************************************/
  /*!
  @brief Creates an ActionCall.
  @param set A reference to the ActionSet that this action belongs to.
  @param fn A reference to the function that will be added.
  */
  /**************************************************************************/
  void Actions::Call(ActionSetPtr set, void* fn)
  {
    // Construct the action call
    //ActionPtr call(new ActionCall(set, fn));
  }

  /**************************************************************************/
  /*!
  @brief Creates an ActionDelay
  @param set A reference to the ActionSet that this action belongs to.
  @param duration How long should the delay run for.
  */
  /**************************************************************************/
  void Actions::Delay(ActionSetPtr set, Real duration)
  {
    // Construct the action and add it to the action set
    ActionPtr call(new ActionDelay(set, duration));
    // Add it to the set
    set->Add(call);
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
    auto timeLeft = Duration - Elapsed;
    Elapsed += dt;
    if (Elapsed >= Duration)
      IsFinished = true;

    if (DCE_TRACE_ACTIONS_UPDATE)
      DCTrace << "ActionDelay::Update: dt = '" << dt << "', timeLeft = '" << (dt - timeLeft) << "' \n";
    return std::max(0.0f, dt - timeLeft);
  }

}