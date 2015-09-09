/******************************************************************************/
/*!
\file   Event.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/17/2015
\brief  Events are the main way that objects and components communicate with one
        another in the engine. When a component has data it wants to share, the 
        easiest way is to package the information into an Event, then broadcast
        it. Then any component that wants to have that information can listen
        to it by 'Connecting' a member function to the event.

*/
/******************************************************************************/

#pragma once
#include <string>
#include <memory>
#include <vector>

#include "Delegate.h"

namespace DCEngine {

  class Component;

  /* Pointer-to-member-functions "http://yosefk.com/c++fqa/function.html#fqa-33.1"
     You cannot cast a function pointer to void*.
     If you want a function pointer to point to a member function you
     must declare the type as 'ReturnType (ClassType::*)(ParameterTypes...)'
     Example: using function = void(*)(void);  */

  /* http://blog.molecular-matters.com/2011/09/19/generic-type-safe-delegates-and-events-in-c/ */
  using function = void(Component::*)(int);

    class Event {

    public:

    private:

      std::string _eventId;


    };

  using EventPtr = std::shared_ptr<Event>;
  using EventVec = std::vector<EventPtr>;

}