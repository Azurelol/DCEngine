/******************************************************************************/
/*!
\file   MouseDown.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/16/2015
\brief  The keyboard input event for when the mouse button is released.
*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {

  namespace Events {
    class MouseUp : public Event {
    public:
		int x;
		int y;
    private:

    };
  }


}