/******************************************************************************/
/*!
@file  Event.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "Event.h"

namespace DCEngine{

  // Initialize static member variables
  unsigned int Event::EventsCreated = 0;
  unsigned int Event::EventsDestroyed = 0;

}