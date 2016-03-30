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
      EngineSaveConfigurations() : Event("EngineSaveConfigurations") {}
    };

    class EngineExit : public Event {
    public:
      EngineExit() : Event("EngineExit") {}
    public:
    };

    class EnginePause : public Event {
    public:
      EnginePause() : Event("EnginePause") {}
    };

    class EngineResume : public Event {
    public:
      EngineResume() : Event("EngineResume") {}
    };

    class EnginePauseMenu : public Event {
    public:
      EnginePauseMenu() : Event("EnginePauseMenu") {}
    };

    class EngineInitialized : public Event {
    public:
      EngineInitialized() : Event("EngineInitialized") {}
    };

    class WindowLostFocus : public Event {
    public:
      WindowLostFocus() : Event("WindowLostFocus") {}
    };

    class WindowGainedFocus : public Event {
    public:
      WindowGainedFocus() : Event("WindowGainedFocus") {}
    };

    class GraphicsCompileShaders : public Event {
    public:
      GraphicsCompileShaders() : Event("GraphicsCompileShaders") {}
    };

    class GraphicsCompileShadersError : public Event {
    public:
      GraphicsCompileShadersError() : Event("GraphicsCompileShadersError") {}
      std::string ErrorMessage;
    };

    class GraphicsToggleLightning : public Event {
    public:
      GraphicsToggleLightning() : Event("GraphicsToggleLightning") {}
    };

    class ScriptingCompileScripts : public Event {
    public:
      ScriptingCompileScripts() : Event("ScriptingCompileScripts") {}
    };


  }
}