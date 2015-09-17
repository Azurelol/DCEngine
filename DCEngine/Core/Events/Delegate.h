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

  // Alias declarations
  using ComponentFnPtr = void(Component::*)(DCEngine::Event*);
  
  // Class declaration

  class BaseDelegate {
  public:
    BaseDelegate() {}
    virtual ~BaseDelegate() {};
    virtual void Call() = 0;

  };

  class Delegate {
  public:
    Delegate(void) : componentPtr(NULL) {}
    
    /**************************************************************************/
    /*!
    \brief  Creates the delegate.
    \param  A pointer to the component.
    \param  A pointer to the component's member function.
    */
    /**************************************************************************/
    template <typename GenericComponent, typename EventClass, typename MemberFunction>
    void Create(GenericComponent* component, EventClass eventclass, MemberFunction fn) {
      componentPtr = component;
      //auto eventClass = std::type_index(typeid(EventClass));
      //auto newPtr = static_cast<void<Component::*><((std::type_index(typeid(EventClass)))*)>(fn);
      //untypedPtr = fn;

      funcPtr = static_cast<void(Component::*)(Event*)>(fn);
    }

    /**************************************************************************/
    /*!
    \brief  Calls the component's member function.
    \param  A pointer to the event object to be passed into the component's 
            member function.
    */
    /**************************************************************************/
    void Call(DCEngine::Event* eventObj) {
      // Test: Cast the event object pointer back into its derived class
      //auto derivedEventPtr = dynamic_cast<*>(eventObj);
     //auto derivedEventPtr = reinterpret_cast<void(*)(Event*)>(eventObj);      
     //(componentPtr->*funcPtr)(derivedEventPtr);

      // Works
      (componentPtr->*funcPtr)(eventObj);
    }

    Component* componentPtr;
    ComponentFnPtr funcPtr;
    void* untypedPtr;
    std::type_index classType = typeid(int*);
  };



}