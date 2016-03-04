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

    class EngineSaveConfigurations : public Event {
    public:
    };

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

    class EngineInitialized : public Event {
    public:
    };

    class WindowLostFocus : public Event {
    public:
    };

    class WindowGainedFocus : public Event {
    public:
    };

    class GraphicsCompileShaders : public Event {
    };

    class GraphicsToggleLightning : public Event {
    };

    class ScriptingCompileScripts : public Event {
    };


  }
}