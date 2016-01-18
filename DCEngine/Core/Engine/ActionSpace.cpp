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

namespace DCEngine {

  /*================*
  *   ActionSpace   *
  *================*/
  /**************************************************************************/
  /*!
  @brief Adds an action onto the ActionSpace so that it can be updated.
  @param action A pointer to the action.
  */
  /**************************************************************************/
  void ActionSpace::Add(ActionPtr action)
  {
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
    for (auto& action : AllActions) {
      // If the space of the entity to which this action belongs to is paused
      // do not update the action  

      // Update the action
      action->Update(dt);
    }
    // Clean up any inactive actions (finished, stopped)
    Sweep();
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
  @brief Updates an entity's actions. Updating all the actions one tier below
         in parallel.
  @param dt The time to be updated.
  @return How much time was consumed while updating.
  */
  /**************************************************************************/
  float ActionsOwner::Update(float dt)
  {
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