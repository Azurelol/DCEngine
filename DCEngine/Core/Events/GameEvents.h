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
      ZilchDeclareDerivedType(GameFocusIn, Event);
    };

    class GameFocusOut : public Event {
    public:
      ZilchDeclareDerivedType(GameFocusOut, Event);
    };

    class GameLoad : public Event {
    public:
      ZilchDeclareDerivedType(GameLoad, Event);
    };

    class GameRequestQuit : public Event {
    public:
      ZilchDeclareDerivedType(GameRequestQuit, Event);
    };

    class GameStarted : public Event {
    public:
      ZilchDeclareDerivedType(GameStarted, Event);
    };

    class GameEnded : public Event {
    public:
      ZilchDeclareDerivedType(GameEnded, Event);
    };

    class GameSetup : public Event {
    public:
      ZilchDeclareDerivedType(GameSetup, Event);
    };

  }
}