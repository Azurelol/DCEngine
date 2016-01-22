/******************************************************************************/
/*!
@file   IState.h
@author Jason Jorgenson
@par    email: Jason.Jorgenson\@digipen.edu
@date   1/16/2016
@brief  Base class for States
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once

namespace DCEngine
{
  template <class entity_type>
  class IState
  {
  public:
    /**************************************************************************/
    /*!
    @brief Enter is called once when the state is first entered. 
           Called from StateMachine::ChangeState
    */
    /**************************************************************************/
    virtual void Enter(entity_type *entity) = 0;

    /**************************************************************************/
    /*!
    @brief Update is called once per AI update cycle
    */
    /**************************************************************************/
    virtual void Update(entity_type *entity) = 0;

    /**************************************************************************/
    /*!
    @brief Exit is called once when the state ends. 
           Called from StateMachine::ChangeState
    */
    /**************************************************************************/
    virtual void Exit(entity_type *entity) = 0;

    virtual ~IState() {}
  };
}