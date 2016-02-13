/*****************************************************************************/
/*!
@file      ActionsOwner.cpp
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

  /*===================*
  *   ActionsOwner     *
  *====================*/
  /**************************************************************************/
  /*!
  @brief ActionsOwner constructor.
  @param owner A reference to the owner of this ActionsOwner.
  */
  /**************************************************************************/
  ActionsOwner::ActionsOwner(Entity & owner) : ActionSet("ActionsOwner"), Owner(owner)
  {
    if (!DCE_ACTIONS_ENABLED)
      return;

    if (!ActionSpace::PropagateUpdateDirectly)
      Register();
  }

  /**************************************************************************/
  /*!
  @brief ActionsOwner destructor.
  */
  /**************************************************************************/
  ActionsOwner::~ActionsOwner()
  {
    if (!DCE_ACTIONS_ENABLED)
      return;

    if (!ActionSpace::PropagateUpdateDirectly)
      Deregister();
  }

  /**************************************************************************/
  /*!
  @brief Registers this ActionsOwner to the ActionSpace.
  */
  /**************************************************************************/
  void ActionsOwner::Register()
  {
    Daisy->Register(this);
  }

  void ActionsOwner::Deregister()
  {
    Daisy->Deregister(this);
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
    //DCTrace << " Updating the actionsOwner! \n";

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
      if (timeElapsed <= dt && action->Finished()) {
        // Mark the action to be cleared
        InactiveActions.push_back(action);
      }
    }

    auto& a = InactiveActions;

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