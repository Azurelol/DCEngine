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

  unsigned Action::Created = 0;
  unsigned Action::Destroyed = 0;

  /*!************************************************************************\
  @brief  Action Zilch Definition
  \**************************************************************************/
  ZilchDefineType(Action, "Action", DCEngineCore, builder, type) {
    // Constructor
    //ZilchBindConstructor(builder, type, Action, "type", std::string);
    //ZilchBindDestructor(builder, type, Action);
  }

  ZilchDefineType(ActionSet, "ActionSet", DCEngineCore, builder, type) {
    // Constructor
    //ZilchBindConstructor(builder, type, ActionSet, "type", std::string);
    //ZilchBindDestructor(builder, type, ActionSet);
  }

  ZilchDefineType(ActionSequence, "ActionSequence", DCEngineCore, builder, type) {
    // Constructor
    ZilchBindConstructor(builder, type, ActionSequence, ZilchNoNames);
    ZilchBindDestructor(builder, type, ActionSequence);
  }

  ZilchDefineType(ActionGroup, "ActionGroup", DCEngineCore, builder, type) {
    // Constructor
    ZilchBindConstructor(builder, type, ActionGroup, ZilchNoNames);
    ZilchBindDestructor(builder, type, ActionGroup);
  }

  ZilchDefineType(ActionsOwner, "ActionsOwner", DCEngineCore, builder, type) {
    // Constructor
    ZilchBindConstructor(builder, type, ActionsOwner, "owner", Entity&);
    ZilchBindDestructor(builder, type, ActionsOwner);
  }


  /*!************************************************************************\
  @brief  Actions Zilch Definition
  \**************************************************************************/
  ZilchDefineType(Actions, "Actions", DCEngineCore, builder, type) {
    // Methods
    ZilchBindMethod(builder, type, &Actions::Sequence, ZilchNoOverload, "Sequence", "owner");
    ZilchBindMethod(builder, type, &Actions::Group, ZilchNoOverload, "Group", "owner");
    ZilchBindMethod(builder, type, &Actions::Delay, ZilchNoOverload, "Delay", "sets, duration");
  }

  /**************************************************************************/
  /*!
  @brief Action constructor
  */
  /**************************************************************************/
  Action::Action(std::string type) : Type(type), Elapsed(0.0f), Duration(0.0f), ID(Created++)
  {
  }

  /**************************************************************************/
  /*!
  @brief Action destructor
  */
  /**************************************************************************/
  Action::~Action()
  {
    Destroyed++;
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