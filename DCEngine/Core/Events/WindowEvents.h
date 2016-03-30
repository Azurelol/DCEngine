/******************************************************************************/
/*!
\file   WindowEvents.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   11/6/2015
\brief  These are internal events from the engine.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {
  namespace Events {

    class SetWindowCaption : public Event {
    public:
      SetWindowCaption() : Event("SetWindowCaption") {}
      std::string Caption;
    };

  

  }
}