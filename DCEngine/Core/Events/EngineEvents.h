/******************************************************************************/
/*!
\file   EngineEvents.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   11/6/2015
\brief  These are internal events from the engine.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {
  namespace Events {

    class EngineExit : public Event {
    public:
    };

    class EnginePause : public Event {
    public:
    };

    class EngineResume : public Event {
    public:
    };

    class EnginePauseMenu : public Event {
    public:
    };

  }
}