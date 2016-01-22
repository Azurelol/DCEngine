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
    // Propagate updates directly to all actions.
    if (PropagateUpdateDirectly)
      PropagateDirectly(dt);
    // Propagate updates through all entities
    else
      PropagateThroughOwners(dt);
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

  /*===================*
  *   ActionsOwner     *
  *====================*/
  /**************************************************************************/
  /*!
  @brief ActionsOwner constructor.
  @param owner A reference to the owner of this ActionsOwner.
  */
  /**************************************************************************/
  ActionsOwner::ActionsOwner(Entity & owner) : Owner(owner)
  {
  }

  /**************************************************************************/
  /*!
  @brief Updates an entity's actions. Updating all the actions one tier below
         in parallel.
  @param dt The time to be updated.
  @return How much time was consumed while updating.
  */
  /**************************************************************************/
  float ActionsOwner::Update(float dt)
  {
    DCTrace << " updating!!! \n";

    float mostTimeElapsed = 0;
    // In an ActionGroup, every action is updated in parallel, given the same 
    // time slice.
    for (auto& action : ActiveActions) {
      auto timeElapsed = action->Update(dt);
      // If this action took longer than the previous action, it is the new maximum
      if (timeElapsed > mostTimeElapsed)
        mostTimeElapsed = timeElapsed;
      // If the action was completed (Meaning that it was completed in less time
      // than the time slice given)
      if (timeElapsed <= dt) {
        // Mark the action to be cleared
        InactiveActions.push_back(action);
      }
    }

    // Sweep all inactive actions
    Clear();
    // The time consumed while updating was the maximum time it took
    return mostTimeElapsed;
  }

  /**************************************************************************/
  /*!
  @brief An entity's actions are never finished!
  */
  /**************************************************************************/
  bool ActionsOwner::Validate()
  {
    return false;
  }

}