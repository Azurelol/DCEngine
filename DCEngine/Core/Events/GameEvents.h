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

    class SpaceInitialized : public Event {
    public:
      SpaceInitialized() : Event("SpaceInitialized") {}
      String Name;
      //ZilchDeclareDerivedType(SpaceInitialized, Event);
    };

    class GameFocusIn : public Event {
    public:
      GameFocusIn() : Event("GameFocusIn") {}
      ZilchDeclareDerivedType(GameFocusIn, Event);
    };

    class GameFocusOut : public Event {
    public:
      GameFocusOut() : Event("GameFocusOut") {}
      ZilchDeclareDerivedType(GameFocusOut, Event);
    };

    class GameLoad : public Event {
    public:
      GameLoad() : Event("GameLoad") {}
      ZilchDeclareDerivedType(GameLoad, Event);
    };

    class GameRequestQuit : public Event {
    public:
      GameRequestQuit() : Event("GameRequestQuit") {}
      ZilchDeclareDerivedType(GameRequestQuit, Event);
    };

    class GameStarted : public Event {
    public:
      GameStarted() : Event("GameStarted") {}
      ZilchDeclareDerivedType(GameStarted, Event);
    };

    class GameEnded : public Event {
    public:
      GameEnded() : Event("GameEnded") {}
      ZilchDeclareDerivedType(GameEnded, Event);
    };

    class GameSetup : public Event {
    public:
      GameSetup() : Event("GameSetup") {}
      ZilchDeclareDerivedType(GameSetup, Event);
    };

  }
}