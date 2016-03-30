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
#include "Event.h"
#include "../Binding/CoreBinding.h"
#include "../Binding/CoreBindingTypes.h"

namespace DCEngine{

  // Initialize static member variables
  unsigned int Event::EventsCreated = 0;
  unsigned int Event::EventsDestroyed = 0;

  // Base Event class
  ZilchDefineType(Event, "Event", DCEngineCore, builder, type)
  {    
    DCE_BINDING_SET_HANDLE_TYPE_POINTER;
    ZilchBindConstructor(builder, type, Event, "name", std::string);
    ZilchBindDestructor(builder, type, Event);
    ZilchBindField(builder, type, &Event::Name, "Name", Zilch::PropertyBinding::Get);
    ZilchBindField(builder, type, &Event::EventID, "EventID", Zilch::PropertyBinding::Get);
  }
  
  Event::Event(std::string name) : Name(name) {
    EventsCreated++;
  }


}