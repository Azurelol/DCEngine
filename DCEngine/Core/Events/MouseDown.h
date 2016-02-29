/******************************************************************************/
/*!
\file   MouseDown.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/16/2015
\brief  The keyboard input event for when the mouse button is pressed.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {

  namespace Events {
    class MouseDown : public Event {
    public:
      ZilchDeclareDerivedType(MouseDown, Event);
    Vec2 Position;
    MouseButton ButtonPressed;
    private:

    };
  }


}