/******************************************************************************/
/*!
\file   EditorEvents.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   11/6/2015
\brief  Events for the editor system.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {
  namespace Events {

    class ScriptingLibraryPatched : public Event {
    public:
      ScriptingLibraryPatched() : Event("ScriptingLibraryPatched") {}
    };

    class ScriptingLibraryAboutToCompile : public Event {
    public:
      ScriptingLibraryAboutToCompile() : Event("ScriptingLibraryAboutToCompile") {}
    };


    class ScriptingLibraryCompiled : public Event {
    public:
      ScriptingLibraryCompiled() : Event("ScriptingLibraryCompiled") {}
    };

    class ScriptingErrorMessage : public Event {
    public:
      ScriptingErrorMessage() : Event("ScriptingErrorMessage") {}
      std::string Message;
    };

    class ScriptingLibraryCompilationFailure : public Event {
    public:
      ScriptingLibraryCompilationFailure() : Event("ScriptingLibraryCompilationFailure") {}
    };



  }
}