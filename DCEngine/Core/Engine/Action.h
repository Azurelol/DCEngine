/*****************************************************************************/
/*!
@file      Action.h
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      8/6/2015
@brief     An action...           
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
@note      Reference: http://gamedevelopment.tutsplus.com/tutorials/the-action-list-data-structure-good-for-ui-ai-animations-and-more--gamedev-9264

*/
/******************************************************************************/
#pragma once

#include "ActionList.h"
#include "Delegate.h"

namespace DCEngine {


  
  enum class Ease {
    Linear,
    QuadIn,
    QuadInOut,
    QuadOut,
    SinIn,
    SinInOut,
    SinOut,
  };

  /*===================*
  *     Action        *
  *===================*/
  class Action {
  public:
    virtual float Update(float dt) = 0;
  private:    
  };
  using ActionPtr = std::shared_ptr<Action>;

  /*===================*
  *     ActionSet      *
  *===================*/  
  class ActionSet : public Action {
  public:
    std::vector<ActionPtr> ActiveActions;
    std::vector<ActionPtr> InactiveActions;
    virtual float Update(float dt) = 0;
    void Clear();    
  };
  using ActionSetPtr = std::shared_ptr<ActionSet>;

  class ActionGroup : public ActionSet {
  public:
    float Update(float dt);
  };

  class ActionSequence : public ActionSet {
  public:
    float Update(float dt);
  };
  
  /*===================*
  *     Actions        *
  *===================*/
  class Call : public Action {
  public:
    Call(ActionSet& sequence, EventDelegate* funcPtr);
    float Update(float dt);
    EventDelegate* FunctionPtr;
  };

  class Delay : public Action {
  public:
    float Update(float dt);
  };

  class Property : public Action {
  public:
    float Update(float dt);
  };

  /*===================*
  *     Interface      *
  *===================*/
  // Static class for constructing and using actions.
  class Actions {
    // Constructs an action sequence, adding it to 
    static ActionSetPtr Sequence(ActionSet& owner);
    static void Call(ActionSet& set, EventDelegate& fn);
    static void Delay(ActionSet& set, Real duration);
    template <typename Property, typename EndValue>
    static void Property(ActionSequence& seq, Property prty, EndValue val, Ease ease);

  };


  //template<typename Property, typename EndValue>
  //inline void Action::Property(ActionSequence & seq, Property prty, EndValue val, Ease ease)
  //{
  //
  //}

}