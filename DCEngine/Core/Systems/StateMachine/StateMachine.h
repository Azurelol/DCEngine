/******************************************************************************/
/*!
@file   StateMachine.h
@author Jason Jorgenson
@par    email: Jason.Jorgenson\@digipen.edu
@date   1/16/2016
@brief  Handles State Machine behavior
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "IState.h"

namespace DCEngine
{
  template <class entity_type>
  class StateMachine
  {
  private:

    // A pointer to the agent that owns this instance
    entity_type *owner;

    IState<entity_type> *currentState;

    // A record of the last state the agent was in
    IState<entity_type> *previousState;

    // This state logic is called every time the FSM is updated
    IState<entity_type> *globalState;

  public:
    /**************************************************************************/
    /*!
    @brief StateMachine Constructor
    */
    /**************************************************************************/
    StateMachine(entity_type *owner) : owner(owner), currentState(NULL), previousState(NULL), globalState(NULL) {}

    /**************************************************************************/
    /*!
    @brief StateMachine destructor
    */
    /**************************************************************************/
    ~StateMachine()
    {
      if (previousState != currentState && previousState)
        delete previousState;
      if (currentState)
        delete currentState;
      if (globalState)
        delete globalState;
    }

    /**************************************************************************/
    /*!
    @brief Sets the current state
    */
    /**************************************************************************/
    void SetCurrentState(IState<entity_type> *state)
    {
      currentState = state;
    }

    /**************************************************************************/
    /*!
    @brief Sets the global state
    */
    /**************************************************************************/
    void SetGlobalState(IState<entity_type> *state)
    {
      globalState = state;
    }

    /**************************************************************************/
    /*!
    @brief Sets the previous state
    */
    /**************************************************************************/
    void SetPreviousState(IState<entity_type> *state)
    {
      previousState = state;
    }

    /**************************************************************************/
    /*!
    @brief Updates the global and current state
    */
    /**************************************************************************/
    void Update() const
    {
      // If a global state exists, call its update method
      if (globalState)
        globalState->Update(owner);

      // Same for the current state
      if (currentState)
        currentState->Update(owner);
    }

    /**************************************************************************/
    /*!
    @brief Changes to a new state
    \param The new state 
    */
    /**************************************************************************/
    void ChangeState(IState<entity_type> *newState)
    {
      if (newState == NULL)
        throw DCException("<StateMachine::ChangeState>: Trying to change to a null state");

      if (previousState)
        delete previousState;

      // Keep a record of the previous state
      previousState = currentState;

      // Call the exit method of the existing state, if it exists
      if (currentState != NULL)
        currentState->Exit(owner);

      // Change the state to the new state
      currentState = newState;

      // Call the entry method of the new state
      currentState->Enter(owner);
    }

    /**************************************************************************/
    /*!
    @brief Reverts to the previous state
    */
    /**************************************************************************/
    void RevertToPreviousState()
    {
      ChangeState(previousState);
    }

    // Accessors
    IState<entity_type> *CurrentState() const
    {
      return currentState;
    }

    IState<entity_type> *GlobalState() const
    {
      return globalState;
    }

    IState<entity_type> *PreviousState() const
    {
      return previousState;
    }

    // Returns true if the current state's type is equal to the type of the classed passed as a parameter
    /**************************************************************************/
    /*!
    @brief Compares type of state with current state
    \param state to compare
    \return True if the types are the same
    */
    /**************************************************************************/
    bool isInState(const IState<entity_type>& state) const
    {
      return TypeOf(currentState) == TypeOf(state);
    }
  };
}