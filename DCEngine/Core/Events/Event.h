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

    class Event {

    public:
      virtual ~Event(void) {}

      unsigned int EventID;

    private:
      


    };

  using EventPtr = std::shared_ptr<Event>;
  using EventVec = std::vector<EventPtr>;

}