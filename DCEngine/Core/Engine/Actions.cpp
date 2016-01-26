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

  unsigned Action::ActionsCreated = 0;
  unsigned Action::ActionsDestroyed = 0;

  /**************************************************************************/
  /*!
  @brief Action constructor
  */
  /**************************************************************************/
  Action::Action() : Elapsed(0.0f), Duration(0.0f), ID(ActionsCreated++)
  {
  }

  /**************************************************************************/
  /*!
  @brief Action destructor
  */
  /**************************************************************************/
  Action::~Action()
  {
    ActionsDestroyed++;
  }

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




}