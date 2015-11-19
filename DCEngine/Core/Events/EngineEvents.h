/******************************************************************************/
/*!
\file   EngineEvents.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   11/6/2015
\brief  These are internal events from the engine.
*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {
  namespace Events {

    class EngineExit : public Event {
    public:
    };

    class EnginePaused : public Event {
    public:
    };

    class EngineResume : public Event {
    public:
    };

  }
}