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

namespace DCEngine{

  // Initialize static member variables
  unsigned int Event::EventsCreated = 0;
  unsigned int Event::EventsDestroyed = 0;

}