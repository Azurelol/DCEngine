/*****************************************************************************/
/*!
@file      Action.cpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      1/7/2016
@brief     An action...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Action.h"

namespace DCEngine {

  /*=======================*
  *     ActionSet          *
  *========================*/
  /**************************************************************************/
  /*!
  @brief  Clears all inactive actions.
  */
  /**************************************************************************/
  void ActionSet::Clear()
  {
    //AllActions.erase(
    //  std::remove_if(AllActions.begin(), AllActions.end(),
    //    // Lambdas, ho!
    //    [&](ActionPtr const& a) {
    //  return std::find(InactiveActions.cbegin(), InactiveActions.cend(), a.get())
    //    != InactiveActions.end(); }), AllActions.end());
    //// Now clear!
    InactiveActions.clear();
  }

  /**************************************************************************/
  /*!
  @brief  Checks whether there are any remaining actions in the ActionSet.
          If there's no more remaining, the set is done.
  */
  /**************************************************************************/
  bool ActionSet::Validate()
  {
    if (ActiveActions.empty())
      IsFinished = true;
    return IsFinished;
  }

  /*=======================*
  *     ActionSequence     *
  *========================*/
  /**************************************************************************/
  /*!
  @brief Updates an ActionSequence, by updating the actions in the sequence
         sequentially.
  @param dt The time to be updated.
  @return How much time was consumed while updating.      
  */
  /**************************************************************************/
  float ActionSequence::Update(float dt)
  {
    auto timeLeft = dt;
    for (auto& action : ActiveActions) {
      // Every action consume time in the time slice given.
      timeLeft -= action->Update(timeLeft);      
      // If the action was completed (Meaning there is time remaining after it was updated)
      // , then mark the action to be cleared.
      if (timeLeft >= 0) {
        InactiveActions.push_back(action);
      }
      // If we have run out of time, break out of the loop!
      if (timeLeft <= 0)
        break;
    }
    // Sweep all inactive actions
    Clear();
    // Validate for remaining actions
    Validate();
    // The time was consumed while updating...
    return dt - timeLeft;
  }

  /*=======================*
  *     ActionGroup     *
  *========================*/
  /**************************************************************************/
  /*!
  @brief Updates an ActionGroup, by updating the actions in the group in
         parallel.
  @param dt The time to be updated.
  @return How much time was consumed while updating.
  */
  /**************************************************************************/
  float ActionGroup::Update(float dt)
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
    // Validate for remaining actions
    Validate();
    // The time consumed while updating was the maximum time it took
    return mostTimeElapsed;
  }








}