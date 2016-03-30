/******************************************************************************/
/*!
@file	  ZilchEvent.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   3/13/2016
@brief
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "../Event.h"

namespace DCEngine {
    
  class ZilchEvent : public Event {
  public:
    ZilchDeclareDerivedType(ZilchEvent, Event);
    ZilchEvent();
    ZilchEvent(std::string name);
    

  };

}