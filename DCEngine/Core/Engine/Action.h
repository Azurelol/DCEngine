/*****************************************************************************/
/*!
@file      Action.h
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      8/6/2015
@brief     An action...           
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
@note      Reference: http://gamedevelopment.tutsplus.com/tutorials/the-action-list-data-structure-good-for-ui-ai-animations-and-more--gamedev-9264
@todo      For some reason I am passing the set to each action's constructor.
           Why am I doing that?
*/
/******************************************************************************/
#pragma once

#include "Delegate.h" // Used by ActionCall
#include "Ease.h"

#define DCE_ACTIONS_ENABLED 1

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
  /**************************************************************************/
  /*!
  @class Action is the base class from which all other actions derive from.
  */
  /**************************************************************************/
  class Action : public Zilch::IZilchObject{
  public:
    ZilchDeclareBaseType(Action, Zilch::TypeCopyMode::ReferenceType);
    Action(std::string type = "Action");
    ~Action();
    virtual float Update(float dt) = 0;
    bool Blocking() { return IsBlocking; }
    bool Finished() { return IsFinished; }
    void Pause() { Paused = !Paused; }
    bool IsPaused() { return Paused; }

    static unsigned Created;
    static unsigned Destroyed;
    std::string Type;

  protected:    
    Real Elapsed = 0.0f;
    Real Duration = 0.0f;
    bool IsBlocking = false; // Whether the action blocks other actions behind it
    bool IsFinished = false; // When finished, the action will be removed.    
    bool Paused = false;
  private:
    unsigned ID;
  };
  
  using ActionPtr = Action*;
  //using ActionPtr = std::shared_ptr<Action>;
  using ActionsContainer = std::vector<ActionPtr>;

  /*===================*
  *     ActionSet      *
  *===================*/  
  class ActionSet;
  using ActionSetPtr = ActionSet*;
  //using ActionSetPtr = std::shared_ptr<ActionSet>;
  /**************************************************************************/
  /*!
  @class The ActionSet is the base class from which all other sets derive.
         Sets such as Sequence, Group and the unique set used by entities.
  */
  /**************************************************************************/
  class ActionSet : public Action {
  public:
    ZilchDeclareDerivedType(ActionSet, Action);
    //virtual void Add(Action& action);
    ActionSet(std::string type = "ActionSet") : Action(type) {}
    virtual void Add(ActionSetPtr set);
    virtual void Add(ActionPtr action);
    virtual float Update(float dt) = 0;
    virtual bool Validate();
  protected:
    std::vector<ActionPtr> ActiveActions;
    std::vector<ActionPtr> InactiveActions;
    void Clear();    
  private:
  };

  /**************************************************************************/
  /*!
  @class An ActionGroup is a type of set that updates all its actions and
         children in parallel, giving each an equal time slice.
  */
  /**************************************************************************/
  class ActionGroup : public ActionSet {
  public:
    ZilchDeclareDerivedType(ActionGroup, Action);
    ActionGroup() : ActionSet("ActionGroup") {}
    float Update(float dt);
  };

  /**************************************************************************/
  /*!
  @class An ActionSequence is a type of set that updates all its actions
         and children in sequence, depleting its time slice as it updates
         each.
  */
  /**************************************************************************/
  class ActionSequence : public ActionSet {
  public:
    ZilchDeclareDerivedType(ActionSequence, Action);
    ActionSequence() : ActionSet("ActionSequence") {}
    float Update(float dt);
  };
    
  /*===================*
  *     Actions        *
  *===================*/
  /**************************************************************************/
  /*!
  @class An ActionCall is a type of action that invokes a function the moment
         immediately after being updated.
  */
  /**************************************************************************/
  class ActionCall : public Action {
  public:
    ActionCall(ActionSetPtr set, Delegate* funcPtr);
    float Update(float dt);
    Delegate* FunctionPtr;
  };

  /**************************************************************************/
  /*!
  @class An ActionDelay is a type of action that blocks all actions behind it
         for a specified amount of time.
  */
  /**************************************************************************/
  class ActionDelay : public Action {
  public:
    ActionDelay(ActionSetPtr set, Real duration);
    float Update(float dt);
  };

  /**************************************************************************/
  /*!
  @class An ActionProperty is a type of action that modifies the value of
         a given property over a specified amount of time, using a specified
         interpolation formula (Ease).
  */
  /**************************************************************************/
  template <typename PropertyType>
  class ActionProperty : public Action {
  public:
    ActionProperty(ActionSetPtr set, PropertyType& prop, PropertyType value, Real duration, Ease ease);
    float Update(float dt);

  private:
    PropertyType& Property;
    PropertyType Difference;
    PropertyType InitialValue;
    PropertyType EndValue;
    Real Duration;
    Ease Ease_;

    Real CalculateEase(Real t);

  };

  /**************************************************************************/
  /*!
  @class An ActionOwner is a container of all actions a particular entity
         has. They propagate updates to all actions attached to it.
  */
  /**************************************************************************/
  class Entity;
  class ActionsOwner : public ActionSet {
  public:
    ZilchDeclareDerivedType(ActionsOwner, ActionSet);
    ActionsOwner(Entity& owner);
    ~ActionsOwner();
    float Update(float dt);
    bool Validate();
    Entity& Owner;

  private:
    
    void Register();
    void Deregister();

  };
  using ActionsOwnerPtr = std::shared_ptr<ActionsOwner>;
  using ActionsOwnerContainer = std::vector<ActionsOwnerPtr>;

  /**************************************************************************/
  /*!
  @class The ActionSpace is the class that manages the updating of all actions.
  */
  /**************************************************************************/
  class ActionSpace {
  public:
    void Add(ActionPtr action);
    void Remove(ActionPtr action);
    void Update(float dt);
    static bool PropagateUpdateDirectly;

    ActionSpace();
    ~ActionSpace();

  private:
    // Update methods
    void PropagateDirectly(float dt);
    void PropagateThroughOwners(float dt);

    void Sweep();
    ActionsOwnerContainer AllActionOwners;
    ActionsContainer AllActions;
    ActionsContainer InactiveActions;
  };

  /*===================*
  *     Interface   *
  *===================*/
  /**************************************************************************/
  /*!
  @class The ActionsClass is the interface class that the client will be using
         for constructing and interacting with actions.
  */
  /**************************************************************************/
  class Actions : public Zilch::IZilchObject{
  public:
    ZilchDeclareBaseType(Actions, Zilch::TypeCopyMode::ReferenceType);
    // Constructs an action sequence, adding it to 
    static ActionSetPtr Sequence(ActionSet& owner);
    static ActionSetPtr Group(ActionSet& owner);
    static void Delay(ActionSetPtr sets, Real duration);
    template <typename Class, typename... Args> static void Call(ActionSetPtr sets, void(Class::*func)(Args...), Class* object, Args...);
    template <typename PropertyType> static void Property(ActionSetPtr sets, PropertyType & propertyRef, PropertyType val, Real duration, Ease ease);

  private:
    
  };

}

#include "Actions.hpp"