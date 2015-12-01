/******************************************************************************/
/*!
@file   GameEvents.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/30/2015
@brief  These are internal events from the engine.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {
  namespace Events {

    class GameFocusIn : public Event {
    public:
    };

    class GameFocusOut : public Event {
    public:
    };

    class GameLoad : public Event {
    public:
    };

    class GameRequestQuit : public Event {
    public:
    };

    class GameStarted : public Event {
    public:
    };

    class GameSetup : public Event {
    public:
    };

  }
}