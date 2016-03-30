/******************************************************************************/
/*!
\file   EventDelegate.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/9/2015
\brief  A delegate is a class that wraps a pointer or reference to an object 
        instance, a member method of that object's class to be called on that
        object instance, and provides a method to trigger that call.
\note   I am too inexperienced a programmer to have been able to come up
        with the implementation on my own. Thus, I researched and ended up
        reading through several implementations:

        http://allenchou.net/2012/04/easy-c-delegates/
        http://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible
        http://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Delegates
        http://www.codeguru.com/cpp/cpp/cpp_mfc/pointers/article.php/c4135/DELEGATES-and-C.htm
        http://codereview.stackexchange.com/questions/14730/impossibly-fast-delegate-in-c11
        
        I also got a lot of help from one of my mentors, Arend Danielek, after
        he walked me through the way he implemented the event system in his Junior
        game, "Coffee Break".

        9/11/2015: I finally finished the delegate implementation, in no small measure
        thanks to Gab and Arend.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.


*/
/******************************************************************************/
#pragma once

// Libraries
#include <list>
#include <functional>
#include <typeindex>
#include <typeinfo>
// Engine
#include "..\Engine\Types.h"

namespace DCEngine {

  // Forward declarations
  class Object;
  class Component;
  class Event;
  class EventDelegate;
  class ZilchComponent;
  namespace Systems {
    class ZilchInterface;
  }
  /**************************************************************************/
  /*!
  @class EventDelegate Base class for delegates used for the event system.
  */
  /**************************************************************************/
  class EventDelegate {
  public:
    EventDelegate() {}
    virtual ~EventDelegate() {};
    virtual bool Call(Event* event) = 0;
    virtual Object* GetObserver() = 0;
  };

  // Containers
  using ObserverRegistryMapStr = std::map < std::string, std::list<std::unique_ptr<EventDelegate>>>;
  using ObserverRegistryMapTypeIndex = std::map<std::type_index, std::list<std::unique_ptr<EventDelegate>>>;
  struct EventDelegatesInfo {
    struct EventDelegateInfo {
      std::string Name;
      std::vector<std::string> Observers;
    };
    std::vector<EventDelegateInfo> Events;
  };

  /**************************************************************************/
  /*!
  @class EventMemberFunctionDelegate Templated class that allows member 
         functions to connect to an entity's events. This version is used
         by C++.
  */
  /**************************************************************************/
  template <typename Class, typename EventClass>
  class EventMemberFunctionDelegate : public EventDelegate {
  public:
    typedef void(Class::*EventFn)(EventClass* event);
    EventFn FuncPtr;
    Class* Inst;

    /**************************************************************************/
    /*!
    @brief  Calls the member function given an event.
    @param  A pointer to the event object.
    @note   If the Object instance is no longer valid (such as if the object
            was destroyed, this will return false)
    */
    /**************************************************************************/
    virtual bool Call(Event* event) {
      EventClass* eventObj = dynamic_cast<EventClass*>(event);
      // If the object instance has been rendered null, do nothing
      if (Inst ==  nullptr || FuncPtr == nullptr) {
        return false;
      }
      // Else, if it is active, dispatch the event object
      (Inst->*FuncPtr)(eventObj);
      return true;
    }
    Object* GetObserver() {
      return Inst;
    }

  };


  //// Zilch version
  //class ScriptDelegate : public EventDelegate {
  //  void Invoke(Event*);
  //  Zilch::Delegate* Delegate;
  //  Zilch::ExecutableState* State;
  //};

  /**************************************************************************/
  /*!
  @class EventZilchFunctionDelegate Templated class that allows member
  functions to connect to an entity's events. This version is used
  by C++.
  */
  /**************************************************************************/
  class EventZilchFunctionDelegate : public EventDelegate {
  public:
    Zilch::Delegate Delegate;
    Zilch::ExecutableState* State;
    virtual bool Call(Event* event);
    Object* GetObserver();
  };







}