/******************************************************************************/
/*!
\file   MouseUp.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/16/2015
\brief  The keyboard input event for when the mouse button is released.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {

  namespace Events {
    class MouseUp : public Event {
    public:
        ZilchDeclareDerivedType(MouseDown, Event);
      Vec2 Position;
      MouseButton ButtonReleased;
    private:

    };
  }


}