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
    bool Blocking() { return IsBlocking; }
    bool Finished() { return IsFinished; }

  protected:    
    Real Elapsed = 0.0f;
    Real Duration = 0.0f;
    bool IsBlocking = false; // Whether the action blocks other actions behind it
    bool IsFinished = false; // When finished, the action will be removed.
  };
  using ActionPtr = std::shared_ptr<Action>;
  using ActionsContainer = std::vector<ActionPtr>;

  /*===================*
  *     ActionSet      *
  *===================*/  
  class ActionSet : public Action {
  public:
    virtual float Update(float dt) = 0;
    virtual bool Validate();
  protected:
    std::vector<ActionPtr> ActiveActions;
    std::vector<ActionPtr> InactiveActions;
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
  class ActionCall : public Action {
  public:
    ActionCall(ActionSet& sequence, Delegate* funcPtr);
    float Update(float dt);
    Delegate* FunctionPtr;
  };

  class ActionDelay : public Action {
  public:
    ActionDelay(ActionSequence& sequence, Real duration);
    float Update(float dt);
  };

  template <typename PropertyType>
  class ActionProperty : public Action {
  public:
    float Update(float dt);

  private:
    PropertyType property;
  };

  /*===================*
  *     Interface      *
  *===================*/
  class EntityActions : public ActionSet {
  public:
    float Update(float dt);
    bool Validate();

  private:
    void Add(ActionPtr action);
    ActionsContainer ActiveActions;

  };

  // Static class for constructing and using actions.
  class Actions {
  public:
    // Constructs an action sequence, adding it to 
    static ActionSetPtr Sequence(ActionSet& owner);
    static ActionSetPtr Group(ActionSet& owner);
    static void Call(ActionSet& set, void* fn);
    static void Delay(ActionSet& set, Real duration);
    template <typename Property, typename EndValue>
    static void Property(ActionSequence& seq, Property prty, EndValue val, Ease ease) {
    }

  private:

  };

  /*===================*
  *     ActionSpace   *
  *===================*/
  class ActionSpace {
  public:
    void Add(ActionPtr action);
    void Remove(ActionPtr action);
    void Update(float dt);

  private:
    ActionsContainer AllActions;
    ActionsContainer InactiveActions;
    void Sweep();

  };

  //template<typename Property, typename EndValue>
  //inline void Action::Property(ActionSequence & seq, Property prty, EndValue val, Ease ease)
  //{
  //
  //}


  /*==============*
  *   Property    *
  *==============*/
  /**************************************************************************/
  /*!
  @brief Updates this action.
  @param dt The time slice given.
  */
  /**************************************************************************/
  template<typename PropertyType>
  inline float ActionProperty<PropertyType>::Update(float dt)
  {
    return 0.0f;
  }

}