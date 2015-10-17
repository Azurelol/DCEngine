/******************************************************************************/
/*!
\file   KeyPressed.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/16/2015
\brief  The keyboard input event for when a key is released.
*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {

  namespace Events {
    class KeyDown : public Event {
    public:
      Keys Key;

      //uint State;
      bool ShiftPressed;

    private:

    };
  }


}