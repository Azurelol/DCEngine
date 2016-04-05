/******************************************************************************/
/*!
\file   GraphicsEvents.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   11/6/2015
\brief  Events for the graphics system.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {
  namespace Events {

    class WindowFullScreenToggle : public Event {
    public:
      bool Enabled;
      WindowFullScreenToggle() : Event("WindowFullScreenToggle"), Enabled(true) {}
    };

    class WindowFullScreenEnabled : public Event {
    public:
      WindowFullScreenEnabled() : Event("WindowFullScreenEnabled") {}
    };

    class WindowFullScreenDisabled : public Event {
    public:
      WindowFullScreenDisabled() : Event("WindowFullScreenDisabled") {}
    };
    
    class WindowResize : public Event {
    public:
      WindowResize() : Event("WindowResize") {}
      Vec2 Dimensions;
    };

  }
}