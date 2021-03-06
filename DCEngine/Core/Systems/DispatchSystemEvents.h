/*****************************************************************************/
/*!
@file   DispatchSystemEvents.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   12/01/2015
@note   This file includes all the headers for the components that Rebound
uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once

namespace DCEngine {
  
  class Object;

  namespace Systems {

    class Editor;

    class DispatchSystemEvents {
    public:

      static void EngineInitialized();
      static void EngineResume();
      static void EnginePause();
      static void EngineExit();
      static void EngineSaveConfigurations();
      
      static void EditorInitialize(Editor& editor);
      static void EditorSelectObject(Object* object);
      static void EditorDeselectObject();
      static void EditorSave();
      static void EditorRebuildZilchComponents();
      
      static void SetWindowCaption(std::string name);
      static void WindowResize(const Vec2&);
			static void WindowRecreate();
      static void WindowFullScreenToggle(bool enabled);
      static void WindowFullScreenEnabled();
      static void WindowFullScreenDisabled();
      static void WindowLostFocus();
      static void WindowGainedFocus();
			static void WindowResizeToNative();
      
      static void GraphicsCompileShaders();
      static void GraphicsCompileShadersError(const std::string& message);
      static void GraphicsToggleLightning();

      static void ScriptingCompile();
      static void ScriptingLibraryPatched();
      static void ScriptingLibraryAboutToCompile();
      static void ScriptingLibraryCompiled();
      static void ScriptingLibraryCompilationFailure();
      static void ScriptingErrorMessage(const std::string& message);

      static void ContentFileMoved(const std::string& fileName);
      static void ContentFileUpdated(const std::string& fileName);
      static void ContentFileFound(const std::string& fileName);
      static void ContentFileDeleted(const std::string& fileName);
      static void ContentFileScanComplete();
      static void ContentProjectLoaded();
      

    };

  }

 

}