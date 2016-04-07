/*****************************************************************************/
/*!
@file      ActionSpace.cpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      1/17/2016
@brief     An action...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Action.h"

#include "Engine.h"

namespace DCEngine {

  /*================*
  *   ActionSpace   *
  *================*/
  /**************************************************************************/
  /*!
  @brief ActionSpace constructor.
  */
  /**************************************************************************/
  bool ActionSpace::PropagateUpdateDirectly = false;
  ActionSpace::ActionSpace()
  {
  }

  /**************************************************************************/
  /*!
  @brief ActionSpace destructor.
  */
  /**************************************************************************/
  ActionSpace::~ActionSpace()
  {
    AllActions.clear();
  }

  /**************************************************************************/
  /*!
  @brief Adds an action onto the ActionSpace so that it can be updated.
  @param action A pointer to the action.
  */
  /**************************************************************************/
  void ActionSpace::Add(ActionPtr action)
  {
    if (DCE_TRACE_ACTIONS_ADD)
      DCTrace << "ActionSpace::Add \n";
    AllActions.push_back(action);
  }
  
  /**************************************************************************/
  /*!
  @brief Removes an action from the ActionSpace.
  @param action A pointer to the action.
  */
  /**************************************************************************/
  void ActionSpace::Remove(ActionPtr action)
  {
    if (DCE_TRACE_ACTIONS_REMOVE)
      DCTrace << "ActionSpace::Remove \n";
    auto actioniter = std::find(AllActions.begin(), AllActions.end(), action);
    //std::swap(AllActions.end(), actioniter);
    //AllActions.pop_back();
    AllActions.erase(actioniter);
  }

  /**************************************************************************/
  /*!
  @brief Updates the ActionSpace, updating every active action.
  @param dt The time slice given.
  */
  /**************************************************************************/
  void ActionSpace::Update(float dt)
  {
    for (auto& action : AllActions) {

      // If it's paused, do not update its actions
      if (action->IsPaused())
        continue;

      // Update the action
      action->Update(dt);
    }


    // Propagate updates directly to all actions.
    //if (PropagateUpdateDirectly)
    //  PropagateDirectly(dt);
    //// Propagate updates through all entities
    //else
    //  PropagateThroughOwners(dt);

    // Clean up any inactive actions (finished, stopped)
    Sweep();
  }

  /**************************************************************************/
  /*!
  @brief Updates all actions
  @param dt The time slice given.
  */
  /**************************************************************************/
  void ActionSpace::PropagateDirectly(float dt)
  {
  }

  /**************************************************************************/
  /*!
  @brief Removes all inactive actions.
  */
  /**************************************************************************/
  void ActionSpace::PropagateThroughOwners(float dt)
  {    
    for (auto& actionOwner : AllActionOwners) {

      // If it's paused, do not update its actions
      if (actionOwner->IsPaused())
        continue;

      // Update the action
      actionOwner->Update(dt);
    }
  }

  /**************************************************************************/
  /*!
  @brief Removes all inactive actions.
  */
  /**************************************************************************/
  void ActionSpace::Sweep()
  {
    for (auto& action : InactiveActions) {

    }
  }





}