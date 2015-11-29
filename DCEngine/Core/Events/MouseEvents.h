/******************************************************************************/
/*!
@file   MouseEvents.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/16/2015
@brief  MouseEvents.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {

  namespace Events {
    
    class MouseEnter : public Event {
    public:
    };

    class MouseExit : public Event {
    public:
    };

    class MouseUpdate : public Event {
    public:
      Vec2 ScreenPosition;
    };

    class MouseClickedOn : public Event {
    public:

    };

  }


}