/******************************************************************************/
/*!
\file   InputEvents.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/16/2015
\brief  The keyboard input event for when a key is released.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {

  enum class MouseWheelScrollDirection {
    Up,
    Down,
  };

  namespace Events {

    class KeyDown : public Event {
    public:
      ZilchDeclareDerivedType(KeyDown, Event);
      KeyDown() : Event("KeyDown") {}
      Keys Key;
      bool ShiftPressed;
    };


    class KeyUp : public Event {
    public:
      ZilchDeclareDerivedType(KeyUp, Event);
      KeyUp() : Event("KeyUp") {}
      Keys Key;
      bool ShiftPressed;
    };

    class MouseDown : public Event {
    public:
      ZilchDeclareDerivedType(MouseDown, Event);
      MouseDown() : Event("MouseDown") {}
      Vec2 Position;
      MouseButton ButtonPressed;
    private:
    };

    class MouseEnter : public Event {
    public:
      ZilchDeclareDerivedType(MouseEnter, Event);
      MouseEnter() : Event("MouseEnter") {}
    };

    class MouseExit : public Event {
    public:
      ZilchDeclareDerivedType(MouseExit, Event);
      MouseExit() : Event("MouseExit") {}
    };

    class MouseUpdate : public Event {
    public:
      ZilchDeclareDerivedType(MouseUpdate, Event);
      MouseUpdate() : Event("MouseUpdate") {}
      Vec2 ScreenPosition;
    };

    class MouseClickedOn : public Event {
    public:
      ZilchDeclareDerivedType(MouseClickedOn, Event);
      MouseClickedOn() : Event("MouseClickedOn") {}
    };



    class MouseScroll : public Event {
    public:
      ZilchDeclareDerivedType(MouseScroll, Event);
      MouseScroll() : Event("MouseScroll"), Delta(0.0f) {}
      Real Delta;
      Vec2 Direction;
    private:

    };

    class MouseUp : public Event {
    public:
      ZilchDeclareDerivedType(MouseDown, Event);
      MouseUp() : Event("MouseUp") {}
      Vec2 Position;
      MouseButton ButtonReleased;
    private:
    };

  }
}