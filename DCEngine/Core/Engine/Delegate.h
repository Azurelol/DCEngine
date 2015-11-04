/******************************************************************************/
/*!
\file   Delegate.h
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

*/
/******************************************************************************/
#pragma once

// Libraries
#include <list> // doubly-linked list
#include <functional>
#include <typeindex>
#include <typeinfo>
#include "..\Engine\Types.h"

namespace DCEngine {

  // Forward declarations
  class Component;
  class Event;
  class Delegate;

  class Delegate {
  public:
    Delegate() {}
    virtual ~Delegate() {};
    virtual void Call(Event* event) = 0;
  };


  /**************************************************************************/
  /*!
  \brief  Templated class that allows component member functions to connect
          to an entity's events.
  */
  /**************************************************************************/
  template <typename ComponentClass, typename EventClass>
  class ComponentFunctionDelegate : public Delegate {
  public:
    typedef void(ComponentClass::*EventFn)(EventClass* event);
    EventFn FuncPtr;
    ComponentClass* CompInst;

    /**************************************************************************/
    /*!
    \brief  Calls the member function given an event.
    \param  A pointer to the event object.
    */
    /**************************************************************************/
    virtual void Call(Event* event) {
      EventClass* eventObj = dynamic_cast<EventClass*>(event);
      (CompInst->*FuncPtr)(eventObj);
    }
  };

  /**************************************************************************/
  /*!
  \brief  Templated class that allows systems' member functions to connect
          to an entity's events.
  */
  /**************************************************************************/
  template <typename SystemClass, typename EventClass>
  class SystemFunctionDelegate : public Delegate {
  public:
    typedef void(SystemClass::*EventFn)(EventClass* event);
    EventFn FuncPtr;
    SystemClass* SystemInst;

    /**************************************************************************/
    /*!
    \brief  Calls the member function given an event.
    \param  A pointer to the event object.
    */
    /**************************************************************************/
    virtual void Call(Event* event) {
      EventClass* eventObj = dynamic_cast<SystemClass*>(event);
      (SystemInst->*FuncPtr)(eventObj);
    }
  };


}