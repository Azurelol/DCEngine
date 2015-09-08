/******************************************************************************/
/*!
\file   KeyboardEvent.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/17/2015
\brief  The keyboard event class.

*/
/******************************************************************************/
#include "Event.h"
#pragma once

namespace DCEngine {

    class KeyboardEvent : public Event {
    public:


      //uint State;
      bool ShiftPressed;

    private:

    };


}