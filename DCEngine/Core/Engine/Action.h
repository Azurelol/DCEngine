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
  /**************************************************************************/
  /*!
  @class Action is the base class from which all other actions derive from.
  */
  /**************************************************************************/
  class Action {
  public:
    Action() : Elapsed(0.0f), Duration(0.0f) {}
    virtual float Update(float dt) = 0;
    bool Blocking() { return IsBlocking; }
    bool Finished() { return IsFinished; }
    void Pause() { Paused = !Paused; }
    bool IsPaused() { return Paused; }

  protected:    
    Real Elapsed = 0.0f;
    Real Duration = 0.0f;
    bool IsBlocking = false; // Whether the action blocks other actions behind it
    bool IsFinished = false; // When finished, the action will be removed.    
    bool Paused = false;
  };
  using ActionPtr = std::shared_ptr<Action>;
  using ActionsContainer = std::vector<ActionPtr>;

  /*===================*
  *     ActionSet      *
  *===================*/  
  class ActionSet;
  using ActionSetPtr = std::shared_ptr<ActionSet>;
  /**************************************************************************/
  /*!
  @class The ActionSet is the base class from which all other sets derive.
         Sets such as Sequence, Group and the unique set used by entities.
  */
  /**************************************************************************/
  class ActionSet : public Action {
  public:
    //virtual void Add(Action& action);
    virtual void Add(ActionSetPtr set);
    virtual void Add(ActionPtr action);
    virtual float Update(float dt) = 0;
    virtual bool Validate();
  protected:
    std::vector<ActionPtr> ActiveActions;
    std::vector<ActionPtr> InactiveActions;
    void Clear();    
  };

  /**************************************************************************/
  /*!
  @class An ActionGroup is a type of set that updates all its actions and
         children in parallel, giving each an equal time slice.
  */
  /**************************************************************************/
  class ActionGroup : public ActionSet {
  public:
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
    ActionProperty(ActionSet& sequence, PropertyType& prop, Real duration, Ease ease);
    float Update(float dt);

  private:
    PropertyType& Property;
    PropertyType& Value;
    Real Duration;
    Ease Ease;
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
    ActionsOwner(Entity& owner);
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
  class Actions {
  public:
    // Constructs an action sequence, adding it to 
    static ActionSetPtr Sequence(ActionSet& owner);
    static ActionSetPtr Group(ActionSet& owner);
    static void Call(ActionSetPtr set, void* fn);
    static void Delay(ActionSetPtr set, Real duration);
    template <typename Property>
    static void Property(ActionSequence& seq, Property& prty, Property val, Real duration, Ease ease);

  private:

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
  @brief ActionProperty constructor.
  @param PropertyType The POD or class type of the property;
  @param set A reference to the set this property is part of.
  @param property A reference to the property to be modified.
  @param duration How long this property runs for.
  @param ease What ease this property uses to calculate the interpolation.
  */
  /**************************************************************************/

  /**************************************************************************/
  /*!
  @brief Updates the property.
  @param dt The time slice given.
  */
  /**************************************************************************/
  template<typename PropertyType>
  inline float ActionProperty<PropertyType>::Update(float dt)
  {
    return 0.0f;
  }

  /*==============*
  *   Actions     *
  *==============*/

}