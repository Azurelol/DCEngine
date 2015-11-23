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
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "Types.h"
#include "Delegate.h"

namespace DCEngine {

    class Event {
    public:
      static unsigned int EventsCreated;
      static unsigned int EventsDestroyed;

      Event() {
        EventsCreated++;
      }
      virtual ~Event(void) {
        EventsDestroyed++;
      }
      unsigned int EventID;

    private:

    };



  using EventPtr = std::shared_ptr<Event>;
  using EventVec = std::vector<EventPtr>;

}