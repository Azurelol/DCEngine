#pragma once
#include <list> // doubly-linked list

namespace DCEngine {

  // FORWARD DECLARATION
  class Entity; // Subject
  class Component; // Observer
  class Event;

  // Try using an alias declaration
  // using void(Component::*DelegateFctn) = (void);

  typedef void(Component::*DelegateFn)(void); // dirty

  class Delegate {
  public:
    Delegate(Component* component, DelegateFn function);
    ~Delegate() {};
    
  private:
    Entity* _subject;
    Component* _observer;
    DelegateFn _function;

    // Double-linked lists for subject, observer ptrs
    std::list<Delegate> _subjectList;
    std::list<Delegate> _observerList;

  };

}