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
  using DelegateFnPtr = void(Delegate::*)(DCEngine::Event*);
  
  // Class declaration
  class Delegate {
  public:
    Delegate(void) : componentPtr(NULL) {}

    //template <typename GenericComponent>
    //void Create(GenericComponent* component, ComponentFnPtr fn) {
    //  componentPtr = component;
    //  functionPtr = reinterpret_cast<DelegateFnPtr>(fn);
    //  caller_ = &Delegate::Invoke<GenericComponent>;
    //}

    template <typename GenericComponent, typename MemberFunction>
    void Create(GenericComponent* component, MemberFunction fn) {
      componentPtr = component;
      
      trace << "Delegate::Create - \n";
      //std::vector<fn> a;

      //static_cast<foo::*>(&derivedFoo::poke));
      funcPtr = static_cast<void(Component::*)(DCEngine::Event*)>(fn);
      

      trace << "Delegate::Create - I did stuff.\n";
      //functionPtr = reinterpret_cast<DelegateFnPtr>(fn);
      //caller_ = &Delegate::Invoke<GenericComponent>;
    }

    void Call(DCEngine::Event* eventObj) {
      //trace << "Delegate::Calling member function\n";
      (componentPtr->*funcPtr)(eventObj);
      //(this->*caller_)(eventObj);
    }

  private:


    template <typename GenericComponent>
    void Invoke(DCEngine::Event* eventObj) {

      if (componentPtr == NULL) {
        trace << "Delegate::Invoke - Fatal: No component provided \n";
      }

      GenericComponent* comp = void_cast<GenericComponent*>(componentPtr);
      (comp->*(reinterpret_cast<ComponentFnPtr>(functionPtr)))(eventObj);
    }

    Component* componentPtr;
    void (Delegate::*caller_)(DCEngine::Event* eventObj);
    void (Delegate::*functionPtr)(DCEngine::Event* eventObj);
    ComponentFnPtr funcPtr;
    
    //std::function<ComponentFnPtr> funcPtr;

  };



}